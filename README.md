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

Contents:
1. Linux and ROS Kinetic
  1.1 Setting up the Linux machine
  1.2 Getting started with TurtleBot simulator
2. Setting up a robot in Linux
  2.1 Solidworks to URDF


### 2. Setting up a robot in ROS
A tutorial can be found at http://wiki.ros.org/Industrial/Tutorials/Create%20a%20URDF%20for%20an%20Industrial%20Robot#Prepare_CAD_Models 

## 2.1 Solidworks to URDF
A .URDF is a type of file used by the ROS archetecture to build your robot in a simulated environment. This .URDF file is composed of joints and links all containing rotation, position, mesh, and parent/child information. Before we write a .URDF, we need to go to Solidworks and save out our robot as a series of .STL files. Note that each rigid body should be its own .STL. 
* Build your assembly until you are happy with it in Solidworks
* Make sure that each link has a coincident mate to its parent link, this will be necessary for future steps
* Save out each link (eg base, proximal_arm_pitch, medial_arm_roll) as its own .STL file. When saving in Solidworks, once STL filetype is selected, there is an options button which will take you to a menu where you can select to save the subassembly as one part and not indidual parts
* give each .STL a name that is all lowercase letters and has no spaces (underscores are okay)
* Send the .STL files to the Linux Machine

### MoveIt! Overview
MoveIt! is a great tool for making the URDF that was exported from SW into a usable model to develop on. 

To upload your URDF model into MoveIt, I would recommend using the Setup Assistant which will help to define joints, collisions, IK solvers, and motion planning. To start the MoveIt! Setup Assistant:

`roslaunch moveit_setup_assistant setup_assistant.launch`
