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
  1.1 Installing and Setting Up ROS
2. Setting up a robot in Linux
  2.1 Solidworks to URDF

# 1. Linux and ROS Kinetic
## Overview
This section will provide you with a brief overview of the Linux architecture, an overview of ROS and and how to correctly setup ROS Kinetic distribution. 
## 1.1 Installing and Setting Up ROS
We will be working on an Ubuntu machine running 16.04 which is fully supported until April 2021. A lot of the commands that we are going to use will be done in the terminal. There are two ways to access the terminal:
* 1. Use Ctrl + Alt + T and it will open a new window
* 2. Go to the search bar at the top left of the desktop and search for terminal. Click on it to open it up

To install ROS, following this official guide: http://wiki.ros.org/kinetic/Installation/Ubuntu
The full desktop instillation should be used. I would recommend setting the .bashrc to the recommended pathway, it can be annoying and troublesome to have to do it with every new terminal. 

Once ROS Kinetic is installed, move on to testing the instillation and setting up a catkin workspace. 
Start with these three tutorials to learn the basics and ensrue that everything is set up properly: 
* http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment 
* http://wiki.ros.org/ROS/Tutorials/NavigatingTheFilesystem
* http://wiki.ros.org/ROS/Tutorials/CreatingPackage

These tutorials will help you setup a catkin workspace in the home folder and build out a tutorial package. This is good to do to ensure that everything was installed correctly. The tutorial package can be deleted later if desired, but the files can be used as good reference templates. 

If this is your first time in ROS, feel free to continue on with the tutuorials. It is a great way to learn the OS and beomce farmiliar with ROS


# 2. Setting up a robot in ROS
Now on to the meat and bones of ROS, setting up an acutal robot. 

A tutorial about setting up an industrial robot can also be found at:
http://wiki.ros.org/Industrial/Tutorials/Create%20a%20URDF%20for%20an%20Industrial%20Robot#Prepare_CAD_Models 

## 2.1 Solidworks to URDF
A .URDF is a type of file used by the ROS archetecture to build your robot in a simulated environment. This .URDF file is composed of joints and links all containing rotation, position, mesh, and parent/child information. Before we write a .URDF, we need to go to Solidworks and save out our robot as a series of .STL files. Note that each rigid body should be its own .STL. 

There are two types of mesh files that are required for the full ROS simulations to work, a visual and collision model. The visual model can be fairly intracate and look good (aka high triangle count) becuase it will just be used for visual rendering. The collision model however should be simpler becuase it will be used for all of the collision calculations and not every hole face will need to be taken into account. We will use a program called MeshLab www.meshlab.net to simplify our collision mesh. 

### Visual Model
* Build your assembly until you are happy with it in Solidworks
* Make sure that each link has a coincident mate to its parent link, this will be necessary for future steps
* Save out each link (eg base, proximal_arm_pitch, medial_arm_roll) as its own .STL file. When saving in Solidworks, once STL filetype is selected, there is an options button which will take you to a menu where you can select to save the subassembly as one part and not indidual parts
* give each .STL a name that is all lowercase letters and has no spaces (underscores are okay)
* Send the visual .STL files to the Linux Machine
### Collision Model
* To make the collision STL's, go to www.meshlab.net and install MeshLab for your OS
* Open MeshLab and click on File -> Import Mesh and choose the STL you wish to import
* To simplify the mesh, go to Fileter -> Remeshing, Simplification and Reconstruction and either choose convex hull or Simplification: Quadric Edge Collapse Decimination. Both have different outcomes, so try both and see what works best for your part
* Export that piece as an STL after it has been simplified and send the collision STLs to the Linux Machine



# MoveIt! Overview
MoveIt! is a great tool for making the URDF that was exported from SW into a usable model to develop on. 

To upload your URDF model into MoveIt, I would recommend using the Setup Assistant which will help to define joints, collisions, IK solvers, and motion planning. To start the MoveIt! Setup Assistant:

`roslaunch moveit_setup_assistant setup_assistant.launch`
