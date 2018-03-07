# ServoArm1
## A Guide to the Development of the Kascope Robotic Arm

### What's Covered: 
The purpose of this document is to help the reader through the process of robotic development using ROS (Robotic operating system) to the point where they feel comfortable configuring, developing, and maintianing the robotic arm. This document will cover a lot of basic ROS topics, but will focus on the 6DOF arm. Other documents will be kept for other robotic platforms. 

### Requirements
* Ubuntu Linux 16.04.3
* Arduino Mega 2560
* Servo Robotic Arm

The following packages:
* ROS Kinetic Distribution
* SW to URDF Exporter
* Gazebo Plugin
* TF Plugin
* MoveIt! Plugin
* Arduino IDE



### MoveIt! Overview
MoveIt! is a great tool for making the URDF that was exported from SW into a usable model to develop on. 

To upload your URDF model into MoveIt, I would recommend using the Setup Assistant which will help to define joints, collisions, IK solvers, and motion planning. To start the MoveIt! Setup Assistant:

`roslaunch moveit_setup_assistant setup_assistant.launch`
