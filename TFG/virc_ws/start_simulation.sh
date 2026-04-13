#!/usr/bin/env bash
"""
VIRC System - Startup Script
Copyright (c) 2026 VIRC. All rights reserved.

Orchestrates the launch of Gazebo, Nav2, the RAI Agent, and the HMI.
"""

# Exit immediately if a command exits with a non-zero status
set -e

# Configuration variables
WS_PATH="$HOME/virc_ws"
VENV_PATH="$WS_PATH/virc_env/bin/activate"

echo "--- Starting VIRC Simulation Environment ---"

# 1. Launch Gazebo (Physical World)
echo "[1/4] Starting Gazebo (World)..."
gnome-terminal -- bash -c "source $VENV_PATH && ros2 launch \
    turtlebot3_gazebo turtlebot3_house.launch.py; exec bash"

sleep 5

# 2. Launch Navigation Stack (Mapping & Localization)
# Note: The tb3_house_map (.yaml and .pgm) was obtained from the Anis Koubaa's 
# open-source repository: https://github.com/aniskoubaa/ros_course_part2
echo "[2/4] Starting Nav2 and RViz..."
gnome-terminal -- bash -c "source $VENV_PATH && ros2 launch \
    turtlebot3_navigation2 navigation2.launch.py use_sim_time:=true \
    map:=/home/lourdes/virc_ws/maps/tb3_house_map.yaml; exec bash"

sleep 5

# 3. Launch RAI Agent (The Brain)
echo "[3/4] Starting RAI Agent Logic..."
gnome-terminal -- bash -c "source $VENV_PATH && \
    python3 $WS_PATH/src/rai_voice_ctrl/rai_voice_ctrl/agent_logic.py; exec bash"

# 4. Launch HMI (The Voice/Text Interface)
echo "[4/4] Starting Human-Machine Interface..."
gnome-terminal -- bash -c "source $VENV_PATH && \
    python3 $WS_PATH/src/rai_voice_ctrl/rai_voice_ctrl/rai_interface.py; exec bash"

echo "--- All systems are online ---"
