#!/usr/bin/env bash
"""
VIRC System - Shutdown Script
Copyright (c) 2026 VIRC. All rights reserved.

Safely terminates all ROS 2 nodes, Python agents, and Gazebo processes.
"""

echo "Shutting down VIRC systems..."

# Terminate ROS 2 launches and Python scripts
# Using -f with specific patterns to avoid killing unrelated processes
pkill -f "ros2 launch" || true
pkill -f "agent_logic.py" || true
pkill -f "rai_interface.py" || true

# Force kill simulators and visualizers
killall -9 gzserver gzclient rviz2 2>/dev/null || true

echo "Cleanup complete. System offline."
