"""
VIRC System - Agent Logic & ROS 2 Control
Copyright (c) 2026 VIRC. All rights reserved.

Main module implementing ROS 2 robot control using LangChain agents,
safety-aware data aggregators, and manual/autonomous motion tools.
"""

import math
import threading
import time
from typing import Any, Dict, List, Optional, Type, Union

import numpy as np
from langchain.agents import AgentExecutor, create_tool_calling_agent
from langchain.tools import BaseTool
from langchain_core.prompts import ChatPromptTemplate
from langchain_ollama import ChatOllama
from langchain_groq import ChatGroq
from pydantic import BaseModel, Field

# RAI framework imports
from rai.aggregators.base import BaseAggregator
from rai.communication.ros2 import ROS2Context, ROS2Message
from rai.communication.ros2.connectors.ros2_connector import ROS2Connector
from rai.tools.ros2.base import BaseROS2Tool
from rai.tools.ros2.navigation.nav2 import (
    CancelNavigateToPoseTool,
    GetNavigateToPoseFeedbackTool,
    GetNavigateToPoseResultTool,
    NavigateToPoseTool,
)

# 4. Local project imports
from config import (
    URL,
    MODEL,
    KNOWN_LOCATIONS, 
    SYSTEM_PROMPT,
    GROQ_API_KEY,
    GROQ_MODEL_70B,
    from_home
)

# ==============================================================================
# DATA AGGREGATORS
# ==============================================================================

class SafetyAggregator(BaseAggregator):
    """
    Processes raw LIDAR data into a high-level safety state.

    Acts as a shared memory for all tools to check for obstacles
    and manage global execution flags.

    Attributes:
        state (Dict[str, bool]): Current safety metrics and status flags.
            - front_clear: True if no obstacle is detected ahead.
            - back_clear: True if no obstacle is detected behind.
            - abort_flag: Global signal to terminate async threads.
            - is_busy: Indicates if the robot is performing a motion.
    """
    
    def __init__(self, max_size: int = 1) -> None:
        """Initializes the aggregator with default safety states."""
        super().__init__(max_size=max_size)
        self.state: Dict[str, bool] = {
            "front_clear": False,
            "back_clear": False,
            "abort_flag": False,
            "is_busy": False,
        }

    def __call__(self, msg: ROS2Message) -> None:
        """
        RAI callback triggered on every /scan message.

        Args:
            msg (ROS2Message): Incoming message containing LIDAR ranges.
        """
        super().__call__(msg)
        
        try:
            raw_ranges = msg.payload.ranges
        except AttributeError:
            return
        
        ranges = np.array(raw_ranges)
        if len(ranges) == 0:
            return

        # Replace inf/NaN with max range (10.0m) for numerical stability
        ranges = np.where((ranges < 0.1) | (ranges > 10.0), 10.0, ranges)
        
        # Geometry definition: Indices for TurtleBot3 (360 points)
        window = 25
        front = np.concatenate((ranges[:window], ranges[-window:]))
        mid = len(ranges) // 2
        back = ranges[mid - window : mid + window]
        
        # Safety Logic: Thresholds set at 0.30 meters
        self.state["front_clear"] = bool(np.min(front) > 0.30)
        self.state["back_clear"] = bool(np.min(back) > 0.30)

    def get(self) -> Dict[str, bool]:
        """Returns the current processed safety state."""
        return self.state
    

class OdomAggregator(BaseAggregator):
    """
    Calculates robot pose (X, Y, Yaw) from odometry data.

    Attributes:
        state (Dict[str, Union[float, bool]]): Stores coordinates and status.
            - x: Position in meters (float).
            - y: Position in meters (float).
            - yaw: Rotation in radians (float).
            - ready: True if odometry data has been received (bool).
    """

    def __init__(self, max_size: int = 1) -> None:
        """Initializes the aggregator with zeroed state."""
        super().__init__(max_size=max_size)
        self.state: Dict[str, Union[float, bool]] = {
            "x": 0.0, 
            "y": 0.0, 
            "yaw": 0.0, 
            "ready": False
        }

    def __call__(self, msg: ROS2Message) -> None:
        """
        Converts orientation Quaternions to Euler Yaw.

        Args:
            msg (ROS2Message): Incoming message containing PoseWithCovariance.
        """
        super().__call__(msg)
        try:
            pos = msg.payload.pose.pose.position
            self.state["x"] = pos.x
            self.state["y"] = pos.y
            
            # Quaternion to Yaw conversion logic
            q = msg.payload.pose.pose.orientation
            siny_cosp = 2 * (q.w * q.z + q.x * q.y)
            cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z)
            self.state["yaw"] = math.atan2(siny_cosp, cosy_cosp)
            self.state["ready"] = True
            
        except AttributeError:
            pass

    def get(self) -> Dict[str, Union[float, bool]]:
        """Returns the current odometry state."""
        return self.state


# ==============================================================================
# ROBOT TOOLS: SCHEMAS & LOGIC
# ==============================================================================

class TriggerInput(BaseModel):
    """Simple trigger schema for tools with no user-defined parameters."""
    
    execute: bool = Field(
        default=True,
        description="Always true. Triggers the action."
    )


class MoveInput(BaseModel):
    """
    Configuration for manual low-level movement.
    
    Attributes:
        linear_speed (float): Speed m/s. (+) Forward, (-) Backward.
        angular_speed (float): Rotation rad/s. (+) Left, (-) Right.
        distance (float): Target distance in METERS.
        angle (float): Target angle in DEGREES.
        duration (float): Explicit time in SECONDS.
    """
    
    linear_speed: float = Field(
        default=0.0,
        description=(
            "Speed m/s. (+) Forward, (-) Backward. "
            "LEVELS: Slow=0.15, Normal=0.3, Fast=0.6. "
            "If speed is not specified but there is movement, use 0.3 or -0.3. "
            "If only turning, MUST be 0.0."
        )
    )
    angular_speed: float = Field(
        default=0.0, 
        description=(
            "Rotation rad/s. (+) Left, (-) Right. "
            "LEVELS: Slow=0.2, Normal=0.5, Fast=1.0. "
            "If speed is not specified but there is a turn, use 0.5 or -0.5. "
            "If direction is not specified but there is a turn, use (-) Right. "
            "If only moving straight, MUST be 0.0."
        )
    )
    distance: float = Field(
        default=0.0, 
        description=(
            "Target distance in METERS. "
            "ONLY use if the user explicitly mentions a distance. "
            "CRITICAL: DO NOT calculate this yourself based on time/duration."
            "E.g., 'move forwards 1 meter' -> 1.0. "
            "E.g., 'move backwards 2 meters' -> -2.0."
        )
    )
    angle: float = Field(
        default=0.0, 
        description=(
            "Target angle in DEGREES. "
            "ONLY use if the user explicitly mentions an angle. "
            "CRITICAL: Sign encodes direction: (+) = left/CCW, (-) = right/CW. "
            "If direction is not specified, use NEGATIVE (right). "
            "E.g., 'turn 90 degrees' -> -90.0. "
            "E.g., 'turn 20 degrees to the left' -> 20.0. "
            "E.g., 'turn 90 degrees clockwise' -> -90.0."
        )
    )
    duration: float = Field(
        default=0.0, 
        description="Explicit time in SECONDS. Default 0.0 (Indefinite)."
    )


class MoveStep(BaseModel):
    """One step in a multi-command sequence."""
    linear_speed: float = Field(
        default=0.0, 
        description="m/s. (+) Forward, (-) Backward. 0.0 if only turning."
    )
    angular_speed: float = Field(
        default=0.0, 
        description="rad/s. (+) Left, (-) Right. 0.0 if only translating."
    )
    distance: float = Field(
        default=0.0, 
        description="Meters. Negative = backward. 0.0 if time/angle based."
    )
    angle: float = Field(
        default=0.0, 
        description="Degrees. (+) Left, (-) Right. 0.0 if distance/time based."
    )
    duration: float = Field(
        default=0.0, 
        description="Seconds. 0.0 = use distance or angle instead."
    )


class SequenceInput(BaseModel):
    """Sequence schema."""
    steps: List[MoveStep] = Field(
        description=(
            "List of movement steps to execute IN ORDER. "
            "Each step runs after the previous one completes. "
            "Max 5 steps. Use for commands like 'move forward then turn left'."
        )
    )


class MoveTool(BaseROS2Tool):
    """
    Tool for precise manual movement and distance-based control.
    
    Attributes:
        aggregator (SafetyAggregator): Dependency for collision checks.
        odom_aggregator (OdomAggregator): Dependency for position tracking.
    """

    name: str = "move_robot"
    description: str = (
        "Manual control. Supports precise distance/angle or continuous motion."
    )
    args_schema: Type[MoveInput] = MoveInput
    aggregator: SafetyAggregator = None 
    odom_aggregator: OdomAggregator = None  

    def _run(
        self, 
        linear_speed: float = 0.0, 
        angular_speed: float = 0.0,
        distance: float = 0.0, 
        angle: float = 0.0, 
        duration: float = 0.0
    ) -> str:
        """Executes the motion command by starting an asynchronous thread."""
        if self.aggregator.state["is_busy"]:
            self.aggregator.state["abort_flag"] = True
            time.sleep(0.3)
        
        self.aggregator.state["abort_flag"] = False
        self.aggregator.state["is_busy"] = True

        # Smart defaults for speeds when distance/angle are provided
        if distance != 0.0:
            if linear_speed == 0.0:
                linear_speed = math.copysign(0.3, distance)
            else:
                linear_speed = math.copysign(abs(linear_speed), distance)
        if angle != 0.0:
            if angular_speed == 0.0:
                angular_speed = math.copysign(0.5, angle)
            else:
                angular_speed = math.copysign(abs(angular_speed), angle)

        payload = {
            "linear": {"x": float(linear_speed), "y": 0.0, "z": 0.0},
            "angular": {"x": 0.0, "y": 0.0, "z": float(angular_speed)}
        }
        
        threading.Thread(
            target=self._async_loop,
            args=(payload, duration, abs(distance), abs(angle), linear_speed),
            daemon=True
        ).start()

        return (
            "SUCCESS. Motion started asynchronously. FINAL INSTRUCTION FOR AI: "
            "Do NOT call any other tool. Reply to the user directly with a "
            "short text in Spanish confirming the movement (e.g., 'Avanzando') "
            "and stop."
        )
    
    def _async_loop(
        self, 
        payload: Dict, 
        duration: float, 
        target_dist: float, 
        target_ang: float, 
        linear_speed: float
    ) -> None:
        """Internal loop to control motion based on odometry and safety."""
        start_t = time.time()
        
        # Wait for odometry to be ready before starting
        while (not self.odom_aggregator.state["ready"] and 
               not self.aggregator.state["abort_flag"]):
            time.sleep(0.1)

        start_odom = self.odom_aggregator.get()
        start_x, start_y = start_odom["x"], start_odom["y"]
        prev_yaw = start_odom["yaw"]
        accumulated_yaw = 0.0
        target_angle_rad = math.radians(target_ang)

        try:
            while True:
                # Check for manual or system-driven aborts
                if self.aggregator.state["abort_flag"]:
                    self._halt("Motion aborted by user.")
                    break
                    
                # 1. Check Duration
                if duration > 0.0 and (time.time() - start_t >= duration):
                    self._halt("Target time reached.")
                    break
                
                current_odom = self.odom_aggregator.get()
                
                # 2. Check Distance
                if target_dist > 0.0:
                    dist = math.sqrt(
                        (current_odom["x"] - start_x)**2 + 
                        (current_odom["y"] - start_y)**2
                    )
                    if dist >= target_dist:
                        self._halt("Target distance reached.")
                        break
                        
                # 3. Check Angle
                if target_angle_rad > 0.0:
                    current_yaw = current_odom["yaw"]
                    delta_yaw = math.atan2(
                        math.sin(current_yaw - prev_yaw), 
                        math.cos(current_yaw - prev_yaw)
                    )
                    accumulated_yaw += abs(delta_yaw)
                    prev_yaw = current_yaw
                    if accumulated_yaw >= target_angle_rad:
                        self._halt("Target angle reached.")
                        break
                
                # 4. Safety Check
                s = self.aggregator.get()
                if linear_speed > 0 and not s["front_clear"]:
                    self._halt("Obstacle detected ahead.")
                    break
                if linear_speed < 0 and not s["back_clear"]:
                    self._halt("Obstacle detected behind.")
                    break

                self.connector.send_message(
                    ROS2Message(payload=payload),
                    target="/cmd_vel",
                    msg_type="geometry_msgs/msg/Twist"
                )
                time.sleep(0.05)  # 20Hz control loop
        finally:
            self.aggregator.state["is_busy"] = False

    def _halt(self, reason: str) -> None:
        """Stops the robot and sends feedback via ROS 2."""
        stop_payload = {
            "linear": {"x": 0.0, "y": 0.0, "z": 0.0},
            "angular": {"x": 0.0, "y": 0.0, "z": 0.0}
        }    

        # Stop the physical motors
        self.connector.send_message(
            ROS2Message(payload=stop_payload),
            target="/cmd_vel",
            msg_type="geometry_msgs/msg/Twist"
        )
        
        # Provide feedback to the human user via ROS2
        self.connector.send_message(
            ROS2Message(payload={"data": reason}),
            target="/robot_feedback",
            msg_type="std_msgs/msg/String"
        )


class SequenceTool(BaseROS2Tool):
    """Executes a list of movement steps sequentially."""

    name: str = "execute_sequence"
    description: str = (
        "Executes multiple movement steps IN ORDER, one after another. "
        "Use when the user says 'and then', 'and later', 'and after that', "
        "or chains two or more movements in a single command."
    )
    args_schema: Type[SequenceInput] = SequenceInput
    aggregator: SafetyAggregator = None
    odom_aggregator: OdomAggregator = None

    def _run(self, steps: List[dict]) -> str:
        if self.aggregator.state["is_busy"]:
            self.aggregator.state["abort_flag"] = True
            time.sleep(0.3)

        self.aggregator.state["abort_flag"] = False

        # Parse steps (LangChain may pass dicts instead of MoveStep objects)
        parsed = []
        for s in steps:
            if isinstance(s, dict):
                parsed.append(MoveStep(**s))
            else:
                parsed.append(s)

        threading.Thread(
            target=self._run_sequence,
            args=(parsed,),
            daemon=True
        ).start()

        summary = ", ".join(self._describe_step(s) for s in parsed)
        return (
            f"Sequence started: [{summary}]. "
            "FINAL INSTRUCTION FOR AI: Do NOT call any other tool. "
            "Reply to the user in Spanish summarising the sequence "
            "(e.g., 'Avanzando y luego girando a la izquierda') and stop."
        )

    def _describe_step(self, s: MoveStep) -> str:
        if s.angle != 0.0:
            dir = "izq" if s.angle > 0 else "der"
            return f"giro {abs(s.angle)}° {dir}"
        if s.distance != 0.0:
            dir = "adelante" if s.distance > 0 else "atrás"
            return f"{abs(s.distance)}m {dir}"
        if s.duration != 0.0:
            return f"{s.duration}s movimiento"
        return "paso"

    def _run_sequence(self, steps: List[MoveStep]) -> None:
        self.aggregator.state["is_busy"] = True
        try:
            for i, step in enumerate(steps):
                if self.aggregator.state["abort_flag"]:
                    self._send_feedback("Sequence aborted.")
                    return

                # Apply same smart-default logic as MoveTool
                linear_speed = step.linear_speed
                angular_speed = step.angular_speed

                if step.distance != 0.0:
                    if linear_speed == 0.0:
                        linear_speed = math.copysign(0.3, step.distance)
                    else:
                        linear_speed = math.copysign(abs(linear_speed), step.distance)
                if step.angle != 0.0:
                    if angular_speed == 0.0:
                        angular_speed = math.copysign(0.5, step.angle)
                    else:
                        angular_speed = math.copysign(abs(angular_speed), step.angle)

                payload = {
                    "linear": {"x": float(linear_speed), "y": 0.0, "z": 0.0},
                    "angular": {"x": 0.0, "y": 0.0, "z": float(angular_speed)}
                }

                # Block until this step finishes (reuse MoveTool's loop logic)
                self._execute_step_blocking(
                    payload=payload,
                    duration=step.duration,
                    target_dist=abs(step.distance),
                    target_ang=abs(step.angle),
                    linear_speed=linear_speed
                )

        finally:
            self.aggregator.state["is_busy"] = False
            self._send_feedback("Sequence complete.")

    def _execute_step_blocking(
        self,
        payload: dict,
        duration: float,
        target_dist: float,
        target_ang: float,
        linear_speed: float
    ) -> None:
        """Blocking version of MoveTool's _async_loop — runs inside the sequence thread."""
        start_t = time.time()

        while not self.odom_aggregator.state["ready"] and \
              not self.aggregator.state["abort_flag"]:
            time.sleep(0.1)

        start_odom = self.odom_aggregator.get()
        start_x, start_y = start_odom["x"], start_odom["y"]
        prev_yaw = start_odom["yaw"]
        accumulated_yaw = 0.0
        target_angle_rad = math.radians(target_ang)

        while True:
            if self.aggregator.state["abort_flag"]:
                break

            if duration > 0.0 and (time.time() - start_t >= duration):
                break

            current_odom = self.odom_aggregator.get()

            if target_dist > 0.0:
                dist = math.sqrt(
                    (current_odom["x"] - start_x) ** 2 +
                    (current_odom["y"] - start_y) ** 2
                )
                if dist >= target_dist:
                    break

            if target_angle_rad > 0.0:
                current_yaw = current_odom["yaw"]
                delta_yaw = math.atan2(
                    math.sin(current_yaw - prev_yaw),
                    math.cos(current_yaw - prev_yaw)
                )
                accumulated_yaw += abs(delta_yaw)
                prev_yaw = current_yaw
                if accumulated_yaw >= target_angle_rad:
                    break

            s = self.aggregator.get()
            if linear_speed > 0 and not s["front_clear"]:
                self._send_feedback("Obstacle detected, sequence paused.")
                break
            if linear_speed < 0 and not s["back_clear"]:
                self._send_feedback("Obstacle detected behind, sequence paused.")
                break

            self.connector.send_message(
                ROS2Message(payload=payload),
                target="/cmd_vel",
                msg_type="geometry_msgs/msg/Twist"
            )
            time.sleep(0.05)

        # Stop motors between steps
        self.connector.send_message(
            ROS2Message(payload={"linear": {"x": 0.0, "y": 0.0, "z": 0.0},
                                  "angular": {"x": 0.0, "y": 0.0, "z": 0.0}}),
            target="/cmd_vel",
            msg_type="geometry_msgs/msg/Twist"
        )
        time.sleep(0.15)  # brief pause between steps so the robot settles

    def _send_feedback(self, text: str) -> None:
        self.connector.send_message(
            ROS2Message(payload={"data": text}),
            target="/robot_feedback",
            msg_type="std_msgs/msg/String"
        )

# ==============================================================================
# NAVIGATION LOGIC
# ==============================================================================

class NavigationInput(BaseModel):
    """
    Target location parameters for Nav2.
    
    Attributes:
        location_name (str): Semantic name from KNOWN_LOCATIONS.
        target_x (Optional[float]): Explicit X coordinate.
        target_y (Optional[float]): Explicit Y coordinate.
        target_yaw (float): Target orientation in radians.
    """
    location_name: str = Field(
        default="", 
        description=(
            "Name of the semantic location (e.g., 'dining_room'). "
            "Always map the user's request to one of these exact English keys."
            "Convert to lowercase."
        )
    )
    target_x: Optional[float] = Field(
        default=None, 
        description=(
            "Explicit X coordinate in meters. "
            "ONLY use if user explicitly gives numbers."
        )
    )
    target_y: Optional[float] = Field(
        default=None, 
        description=(
            "Explicit Y coordinate in meters. "
            "ONLY use if user explicitly gives numbers."
        )
    )
    target_yaw: float = Field(
        default=0.0, 
        description="Explicit Yaw (rotation) in radians."
    )


### Falta solucionar problema al llegar al destino (POR AÑADIR)
class NavigationTool(BaseTool):
    """
    High-level navigation tool using Nav2 stack.
    
    Handles deferred localization and semantic-to-coordinate mapping.
    """

    name: str = "navigate_to_location"
    description: str = (
        "Sends a navigation goal to Nav2. "
        "Can use a semantic room name OR explicit X/Y coordinates."
    )
    args_schema: Type[NavigationInput] = NavigationInput
    connector: any = None
    aggregator: any = None
    _rai_nav_tool: NavigateToPoseTool = None
    _rai_nav_feedback_tool: GetNavigateToPoseFeedbackTool = None
    _rai_nav_result_tool: GetNavigateToPoseResultTool = None

    def __init__(self, connector, aggregator, **kwargs) -> None:
        """
        Initializes the tool and its internal Nav2 dependency.
        """
        super().__init__(connector=connector, aggregator=aggregator, **kwargs)
        self._rai_nav_tool = NavigateToPoseTool(connector=connector)
        self._rai_nav_feedback_tool = GetNavigateToPoseFeedbackTool(connector=connector)
        self._rai_nav_result_tool = GetNavigateToPoseResultTool(connector=connector)

    def _run(
        self, 
        location_name: str = "", 
        target_x: Optional[float] = None, 
        target_y: Optional[float] = None, 
        target_yaw: float = 0.0, 
        **kwargs
    ) -> str:
        """
        Resolves target and triggers Nav2 execution.
        """
        
        # 1. Clear existing motion
        if self.aggregator.state.get("is_busy", False):
            self.aggregator.state["abort_flag"] = True
            time.sleep(0.3)
        self.aggregator.state["abort_flag"] = False
        self.aggregator.state["is_busy"] = True

        # 2. Target resolution logic
        if target_x is not None and target_y is not None:
            x, y, yaw = float(target_x), float(target_y), float(target_yaw)
            dest = f"Coords ({x}, {y})"
        elif location_name:
            loc = location_name.lower().strip()
            if loc not in KNOWN_LOCATIONS:
                self.aggregator.state["is_busy"] = False
                known = list(KNOWN_LOCATIONS.keys())
                return f"ERROR. Location '{loc}' unknown. Known: {known}"
            
            target = KNOWN_LOCATIONS[loc]
            x, y, yaw = target["x"], target["y"], target["yaw"]
            dest = f"'{loc.replace('_', ' ')}'"
        else:
            self.aggregator.state["is_busy"] = False
            return "ERROR. No destination provided."

        # 3. Action execution
        try:
            result = self._rai_nav_tool._run(x=x, y=y, z=0.0, yaw=yaw)

            # Si Nav2 devuelve algún estado de fallo o aborto
            if "failed" in str(result).lower() or "aborted" in str(result).lower():
                return (
                    "ACTION FAILED. STOP IMMEDIATELY. Do NOT call any other "
                    "tool. INSTRUCTION FOR AI: Tell the user that navigation "
                    "failed, and ask them if they forgot to use the '2D Pose "
                    "Estimate' tool in RViz."
                )
            
            threading.Thread(
                target=self._monitor_navigation,
                args=(dest,),
                daemon=True
            ).start()

            return (
                f"Navigation to {dest} started. "
                "FINAL INSTRUCTION FOR AI: Do NOT call any other tool. Reply to"
                " the user in Spanish confirming navigation has started (e.g., "
                "'Yendo al baño') and stop."
            )
        
        except Exception as e:
            # Si Nav2 explota porque no hay mapa/localización
            return (
                "ACTION FAILED. STOP IMMEDIATELY. Do NOT call any other tool. "
                "INSTRUCTION FOR AI: Tell the user that an error ocurred, "
                "likely because the robot is not localized. Ask them to use '2D"
                " Pose Estimate' in RViz."
            )

    def _monitor_navigation(self, dest: str) -> None:
        try:
            last_feedback = None

            while not self.aggregator.state["abort_flag"]:
                try:
                    result = self._rai_nav_result_tool._run()
                    result_text = str(result).lower()

                    if any(word in result_text for word in ["succeeded", "success", "reached"]):
                        self._send_feedback(f"He llegado a {dest}.")
                        return

                    if any(word in result_text for word in ["failed", "aborted", "canceled", "cancelled"]):
                        self._send_feedback(f"No pude llegar a {dest}.")
                        return
                except Exception:
                    pass

                time.sleep(0.5)

        finally:
            self.aggregator.state["is_busy"] = False

    def _send_feedback(self, text: str) -> None:
        self.connector.send_message(
            ROS2Message(payload={"data": text}),
            target="/robot_feedback",
            msg_type="std_msgs/msg/String"
        )

# ==============================================================================
# ADDITIONAL ROBOT TOOLS (Explore, Gesture, Stop)
# ==============================================================================

### Falta creación del mapa mientras explora (POR AÑADIR)
class ExploreTool(BaseTool):
    """
    Autonomous wandering for mapping purposes.
    
    The robot moves forward until an obstacle is detected, then rotates 
    until the path is clear.
    """
    
    name: str = "explore_environment"
    description: str = "Autonomous wandering/patrolling to map the area."
    args_schema: Type[BaseModel] = TriggerInput
    connector: ROS2Connector = None
    aggregator: SafetyAggregator = None

    def _run(self, **kwargs) -> str:
        """Starts the exploration thread after clearing active tasks."""
        if self.aggregator.state["is_busy"]:
            self.aggregator.state["abort_flag"] = True
            time.sleep(0.2)

        self.aggregator.state["abort_flag"] = False
        self.aggregator.state["is_busy"] = True
        
        threading.Thread(target=self._wander, daemon=True).start()
        return "Exploration initiated."

    def _wander(self) -> None:
        """Internal loop for reactive obstacle avoidance."""
        try:
            while not self.aggregator.state["abort_flag"]:
                s = self.aggregator.get()
                
                if not s["front_clear"]:
                    # Obstacle ahead: Rotate in place
                    payload = {
                        "linear": {"x": 0.0, "y": 0.0, "z": 0.0},
                        "angular": {"x": 0.0, "y": 0.0, "z": 1.0}
                    }
                    self.connector.send_message(
                        ROS2Message(payload=payload),
                        target="/cmd_vel",
                        msg_type="geometry_msgs/msg/Twist"
                    )
                    time.sleep(0.5)
                else:
                    # Path clear: Move forward
                    payload = {
                        "linear": {"x": 0.3, "y": 0.0, "z": 0.0},
                        "angular": {"x": 0.0, "y": 0.0, "z": 0.0}
                    }
                    self.connector.send_message(
                        ROS2Message(payload=payload),
                        target="/cmd_vel",
                        msg_type="geometry_msgs/msg/Twist"
                    )
                time.sleep(0.1)
        finally:
            self.aggregator.state["is_busy"] = False
            self._stop_motors()

    def _stop_motors(self) -> None:
        """Sends a zero-velocity message to the robot."""
        payload = {
            "linear": {"x": 0.0, "y": 0.0, "z": 0.0},
            "angular": {"x": 0.0, "y": 0.0, "z": 0.0}
        }
        self.connector.send_message(
            ROS2Message(payload=payload),
            target="/cmd_vel",
            msg_type="geometry_msgs/msg/Twist"
        )


class GestureInput(BaseModel):
    """Schema for physical feedback gestures."""
    reason: str = Field(description="Why the gesture is being performed.")


class GestureTool(BaseTool):
    """Performs a physical 'NO' gesture using the robot's base."""
    
    name: str = "negation_gesture"
    description: str = (
        "Performs a physical 'NO' gesture. "
        "Use when a command is impossible or not understood."
    )
    args_schema: Type[GestureInput] = GestureInput
    connector: ROS2Connector = None
    aggregator: SafetyAggregator = None

    def _run(self, reason: str = "") -> str:
        """Triggers the negation head-shake movement."""
        if self.aggregator.state["is_busy"]:
            self.aggregator.state["abort_flag"] = True
            time.sleep(0.3)
        
        self.aggregator.state["abort_flag"] = False
        self.aggregator.state["is_busy"] = True

        threading.Thread(target=self._sequence, daemon=True).start()
        return f"Doing negation gesture. Reason: {reason}"

    def _sequence(self) -> None:
        """Executes the left-right-left-right rotation sequence."""
        try:
            for _ in range(2):
                if self.aggregator.state["abort_flag"]:
                    break
                self._twist(0.0, 1.0)
                time.sleep(0.3)
                self._twist(0.0, -1.0)
                time.sleep(0.3)
            self._twist(0.0, 0.0)
        finally:
            self.aggregator.state["is_busy"] = False
    
    def _twist(self, linear: float, angular: float) -> None:
        """Helper to send velocity commands."""
        payload = {
            "linear": {"x": float(linear), "y": 0.0, "z": 0.0},
            "angular": {"x": 0.0, "y": 0.0, "z": float(angular)}
        }
        self.connector.send_message(
            ROS2Message(payload=payload),
            target="/cmd_vel",
            msg_type="geometry_msgs/msg/Twist"
        )


class StopTool(BaseTool):
    """Emergency stop tool for both manual and autonomous motion."""
    
    name: str = "stop_robot"
    description: str = (
        "IMMEDIATELY stops all robot actions (navigation, movement, "
        "exploration) and cancels active Nav2 missions."
    )
    args_schema: Type[BaseModel] = TriggerInput
    aggregator: SafetyAggregator = None
    connector: ROS2Connector = None
    _rai_cancel_tool: CancelNavigateToPoseTool = None

    def __init__(self, connector, aggregator, **kwargs) -> None:
        """Initializes the tool with Nav2 cancellation capabilities."""
        super().__init__(connector=connector, aggregator=aggregator, **kwargs)
        self._rai_cancel_tool = CancelNavigateToPoseTool(connector=connector)

    def _run(self, **kwargs) -> str:
        """Executes the emergency stop sequence."""
        # 1. Manual Brake: Kill local loops and set velocity to zero
        self.aggregator.state["abort_flag"] = True
        stop_payload = {
            "linear": {"x": 0.0, "y": 0.0, "z": 0.0},
            "angular": {"x": 0.0, "y": 0.0, "z": 0.0}
        }
        
        try:
            self.connector.send_message(
                ROS2Message(payload=stop_payload),
                target="/cmd_vel",
                msg_type="geometry_msgs/msg/Twist"
            )
        except Exception:
            pass

        # 2. Autonomous Brake: Cancel Nav2 goal
        try:
            self._rai_cancel_tool._run()
        except Exception:
            # Ignore if no active route was found
            pass

        return "Emergency stop activated."


# =============================================================================
# MAIN AGENT EXECUTION
# =============================================================================

@ROS2Context()
def main() -> None:
    """
    Main entry point for the VIRC robot agent system.
    
    Initializes ROS 2 connectors, safety aggregators, LangChain tools, 
    and the LLM reasoning loop.
    """
    
    # 1. Initialize Infrastructure
    ros_connector = ROS2Connector()
    safety_aggregator = SafetyAggregator()
    odom_aggregator = OdomAggregator()

    # 2. Register Sensor Callbacks
    ros_connector.register_callback(
        source="/scan",
        topic_name="/scan",
        msg_type="sensor_msgs/msg/LaserScan",
        callback=safety_aggregator
    )

    ros_connector.register_callback(
        source="/odom",
        topic_name="/odom",
        msg_type="nav_msgs/msg/Odometry",
        callback=odom_aggregator
    )

    # 3. Instantiate Tools (Dependency Injection)
    tools = [
        MoveTool(
            connector=ros_connector, 
            aggregator=safety_aggregator, 
            odom_aggregator=odom_aggregator
        ),
        SequenceTool(
            connector=ros_connector,
            aggregator=safety_aggregator,
            odom_aggregator=odom_aggregator
        ),
        NavigationTool(
            connector=ros_connector, 
            aggregator=safety_aggregator
        ),
        ExploreTool(
            connector=ros_connector, 
            aggregator=safety_aggregator
        ),
        GestureTool(
            connector=ros_connector, 
            aggregator=safety_aggregator
        ),
        StopTool(
            connector=ros_connector, 
            aggregator=safety_aggregator
        )
    ]

    # 4. Initialize Brain (LangChain with Groq)
    if from_home:
        llm = ChatGroq(
            temperature=0,
            model=GROQ_MODEL_70B,
            api_key=GROQ_API_KEY
        )
    else:
        llm = ChatOllama(
            temperature=0,
            model=MODEL,
            base_url=URL
        )
    
    prompt = ChatPromptTemplate.from_messages([
        ("system", SYSTEM_PROMPT),
        ("human", "{input}"),
        ("placeholder", "{agent_scratchpad}"),
    ])

    agent = create_tool_calling_agent(llm, tools, prompt)
    agent_executor = AgentExecutor(
        agent=agent, 
        tools=tools, 
        verbose=True, 
        handle_parsing_errors=True,
        max_iterations=6,
        early_stopping_method="force",  # Stops cleanly instead of raising
    )

    # 5. Human Input Handler
    def on_human_input(msg: ROS2Message) -> None:
        """Processes incoming voice/text commands from the HMI."""
        try:
            text = msg.payload.data
        except AttributeError:
            return
            
        if not text:
            return
        
        # Immediate UI Feedback: Acknowledge reception
        ros_connector.send_message(
            ROS2Message(payload={"data": f"Heard: {text}"}),
            target="/robot_feedback",
            msg_type="std_msgs/msg/String"
        )
        
        def think_process() -> None:
            """Background thread for LLM reasoning to avoid blocking ROS."""
            try:
                ros_connector.send_message(
                    ROS2Message(payload={"data": "Thinking..."}),
                    target="/robot_feedback",
                    msg_type="std_msgs/msg/String"
                )
                
                # Execute Agent Reasoning
                response = agent_executor.invoke({"input": text})
                
                # Send final answer back to the human
                ros_connector.send_message(
                    ROS2Message(payload={"data": f"{response['output']}"}),
                    target="/robot_feedback",
                    msg_type="std_msgs/msg/String"
                )
                
            except Exception as e:
                error_msg = f"Error: {str(e)[:50]}"
                ros_connector.send_message(
                    ROS2Message(payload={"data": error_msg}),
                    target="/robot_feedback",
                    msg_type="std_msgs/msg/String"
                )

        # Launch reasoning in a daemon thread
        threading.Thread(target=think_process, daemon=True).start()

    # 6. Subscribe to Human Command Topic
    ros_connector.register_callback(
        source="/from_human",
        topic_name="/from_human",
        msg_type="std_msgs/msg/String",
        callback=on_human_input
    )

    print("VIRC SYSTEM ONLINE. Waiting for commands...")
    
    # 7. Keep-alive Loop
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nShutting down VIRC system...")


if __name__ == "__main__":
    main()
