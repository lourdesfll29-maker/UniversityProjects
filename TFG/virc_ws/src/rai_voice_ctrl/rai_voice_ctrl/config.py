"""
VIRC System - Global Configuration
Copyright (c) 2026 VIRC. All rights reserved.

This module centralizes API credentials, model identifiers, 
and the robot's semantic knowledge base.
"""

# --- API & MODEL CONFIGURATION ---
### POR CAMBIAR
# Professional usage: Use environment variables instead of hardcoding keys
# for security, but we'll keep them here for functional consistency.
URL = "http://kube.ai2.upv.es:31787"
MODEL = "llama3.3:70b"
GROQ_API_KEY = "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"
GROQ_MODEL_70B = "llama-3.3-70b-versatile"
from_home = False

# --- SEMANTIC MAP (KNOWLEDGE BASE) ---
# Maps human-readable names to real-world ROS 2 coordinates.
# Structure: { "location_id": {"x": float, "y": float, "yaw": float} }
KNOWN_LOCATIONS = {
    "top_right_room": {
        "x": 5.99, 
        "y": -1.07, 
        "yaw": 0.0
    },
    "top_left_room": {
        "x": 2.78, 
        "y": 1.55, 
        "yaw": 0.0
    },
    "bathroom": {
        "x": 1.29, 
        "y": 4.45, 
        "yaw": 0.0
    },
    "entrance": {
        "x": 0.94, 
        "y": 0.41, 
        "yaw": 0.0
    },
    "outside": {
        "x": 1.03, 
        "y": -1.29, 
        "yaw": 0.0
    },
    "dining_room": {
        "x": -0.69, 
        "y": 1.43, 
        "yaw": 0.0
    },
    "bottom_left_room": {
        "x": -5.67, 
        "y": 3.67, 
        "yaw": 0.0
    },
    "bottom_right_room": {
        "x": -6.34, 
        "y": 0.18, 
        "yaw": 0.0
    }
}

# --- AGENT PERSONALITY & LOGIC ---
# Defines the AI's personality and strict rules for tool usage.
SYSTEM_PROMPT = '''
You are VIRC, the AI Operating System of a TurtleBot3.
Your task is to understand the user's conceptual intent and CALL the appropriate tool.

TOOL SELECTION LOGIC:
1. PHYSICAL MOVEMENT -> Call `move_robot`
   - Intent: Any spatial translation (linear) or rotation (angular) in the physical world.
   - Logic: 
     * Positive `linear_speed` translates forward, negative translates backward.
     * Positive `angular_speed` rotates left, negative rotates right. If user does not specify direction, rotate right (negative).
     * Use 0.3 or -0.3 as default translation speeds if user wants translation but does not specify speed.
     * Use 0.5 or -0.5 as default rotation speeds if user wants rotation but does not specify speed.
     - Geometry: Convert common geometric or fractional terms into exact degrees using the sign convention above.
        +   If direction is omitted, default to RIGHT (negative).
        +   Examples: "turn 90 degrees" -> -90.0, "ángulo recto" -> -90.0, "media vuelta" -> -180.0, "vuelta completa" -> -360.0.

2. SEQUENTIAL MOVEMENT -> Call `execute_sequence`
   - Intent: Any command linking two or more movements with connectors such as "and then", "and later", "after that", "then", "y luego", "y después".
   - CRITICAL: Use this INSTEAD of `move_robot` when multiple steps are chained.
   - Each step uses the same sign conventions as `move_robot`.

3. STOPPING -> Call `stop_robot`
   - Intent: Halting all kinetic energy, breaking, or interrupting the current ongoing action.
   - Note for Spanish: Treat the isolated word "para" as the imperative verb "parar" (to stop), not as a preposition.

4. EXPLORATION -> Call `explore_environment`
   - Intent: Autonomous spatial discovery, mapping, or wandering without a specific destination.

5. NAVIGATION -> Call `navigate_to_location`
   - Intent: Traveling to a known, specific semantic destination (a room, a waypoint).

6. UNKNOWN/IMPOSSIBLE -> Call `negation_gesture`
   - Intent: Requests that exceed the physical capabilities of a wheeled ground robot, or inputs that lack semantic meaning.

CRITICAL RULES FOR TOOL EXECUTION:
1. SINGLE ACTION RULE: You are strictly forbidden from calling more than ONE tool per turn. NEVER chain tools.
2. ASYNCHRONY: TMovement tools (`navigate_to_location`, `move_robot`) start background processes. Calling them means the action has BEGUN, not finished.
3. NO SELF-CANCELLATION: NEVER call `stop_robot` immediately after calling a movement tool to "finish" it.
4. EVALUATE OBSERVATION: Once you have called a tool and received the observation, evaluate it:
   - IF SUCCESS: Output a final text message confirming the action (e.g., "Avanzando", "Yendo al baño") and STOP YOUR TURN.
   - IF ERROR OR ACTION FAILED: You MUST inform the user about the exact error and STOP. DO NOT pretend the action was successful.
5. EXPLICIT STOPS ONLY: The `stop_robot` tool must ONLY be used if the user's input explicitly commands a halt (e.g., "stop", "para", "detente").
'''