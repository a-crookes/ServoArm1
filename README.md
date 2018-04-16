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
  2.2 Creating a .xacro model

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
* Open MeshLab and click on File -> Import Mesh and choose the visual STL you wish to import
* To simplify the mesh, go to Fileter -> Remeshing, Simplification and Reconstruction and either choose convex hull or Simplification: Quadric Edge Collapse Decimination. Both have different outcomes, so try both and see what works best for your part
* Export that piece as an STL after it has been simplified and send the collision STLs to the Linux Machine

## 2.2 Setting up the Robot File in ROS
On your Linux machine, you should alredy have a catkin_ws in your home folder. The robot files will live in this file. If the catkin_ws folder was set up correctly, there should then be three folders within that parent folder (src, devel, include). For now, we will only be working in the src folder. 

Navigate into the src folder and create a a new package with the name of your robot inside the <<robot name>> by typing the following into your console
  
  `$ cd ~/catkin_ws/src
   $ catkin_create_pkg <<robot name>> std_msgs rospy roscpp`
  
  *the cd command in the first line navigates controls the terminal navigation to the location you choose (~/catkin_ws/src)*
  This will generate a file of the name of the robot you chose and generate a package.xml and CMakeLists.txt file.  
  
  Info on those can be found here: 
    * http://wiki.ros.org/catkin/package.xml
    * http://wiki.ros.org/catkin/CMakeLists.txt
  
## 2.3 Creating a .xacro file for the Robot
A .xacro file is an XML Macros file, which basically means you can have a bunch of short and readable XML files and then expand them into larger XML files by using macros. 
More xacro documentation can be found at: http://wiki.ros.org/xacro

The xacro files will be stored in the robot arm folder.

## Convert a Xacro file to a URDF file

To convert your .xacro file to .urdf file, start by sourcing your file locaiton to the urdf folder (for example by typing cd catkin_ws/src/<<robot>>/urdf).
Before going straight to a URDF file, we first need to make a xacro file that will combine all of our macros. The one for this robot is called kbot.xacro. Once this xacro is created and/or placed into the urdf folder, we can type the following into the terminal:

`$ rosrun xacro xacro --inorder -o kbot.urdf kbot.xacro`

Next we need to check the model to see if all of the links are correctly chained to their parents. To do this, in the terminal, type: 

`$ check_urdf <<robot.urdf>>`

To view the urdf model, type: 

`$ roslaunch urdf_tutorial display.launch model:=kbot_arm.urdf gui:=True`

### Malformed .stl file error
There is a bug in the .stl files that are saved out from Solidworks that inserts a 'solid' tag into the file which makes it look like an ASCII file instead of the binary file. To fix this, navigate in the terminal to the folder where the stl's are located and type the following: 

`$ sed -i 's/^solid/dilos/' *`

This replaces the instance of solid in the file for the word dilos which is a non-stl defining word and should prevent the file from being considered malformed. 
More information about this bug can be found at https://github.com/ros-industrial/abb_experimental/issues/5

# MoveIt! Overview
MoveIt! is a great tool for making the URDF that was exported from SW into a usable model to develop on. 

To upload your URDF model into MoveIt, I would recommend using the Setup Assistant which will help to define joints, collisions, IK solvers, and motion planning. To start the MoveIt! Setup Assistant:

`roslaunch moveit_setup_assistant setup_assistant.launch`


## Using Scripts in Moveit To Control the Robot
There are various methods to control a robot that I have used, from python script, to cpp script to command line interface. 

To initiate the command line interface: 
Run the moveit demo.launch file by entering

`roslaunch kbot_arm demo.launch` 

Then initiate the moveit commander package and tool with 

`rosrun moveit_commander moveit_commander_cmdline.py`

This will start the command line in the terminal. To work with the robot, first declare which kinematic group you want to work with then type commands. 

Type: 
`use <group name>`
`current` <-- this will give you the current orinetation of the link attached to the end effector
More information on this can be found at: http://docs.ros.org/kinetic/api/moveit_tutorials/html/doc/pr2_tutorials/planning/src/doc/moveit_commander.html

## Arduino and ROS
The arduino rosserial library gives you the ability to use an Arduino board in conjunction with the ROS terminals to publish serial commands. 

### Instructions for Installing the Arduino IDE
Follow these instructions in your terminal to download the Arduino IDE and required libraries

`$ sudo apt-get update`

`$ sudo apt-get install arduino arduino-core`

To install the Arduino ROS software: 

`$ sudo apt-get install ros-kinetic-rosserial-arduino`

`$ sudo apt-get install ros-kinetic-rosseral` 

At this point, we need to source our sketchbook and libraries. The sketchbooks is where we will store our Arduino code during development. ROS bindings for Arduino are implemented as an Arduino library within the IDE. This library, ros_lib, must be added to the /libraries  subdirectory within the user's Arduino sketchbook (the code directory). We are using ~/sketchbook as  the directory in which to store our Arduino code (sketches). The subdirectory  /libraries should already exist, or should be created within your sketchbook . Change to this subdirectory with the following command (<sketchbook>  is the path to your sketchbook  directory):
  
  `$ cd <sketchbook>/libraries` 
  
  `$ rm -rf ros_lib` 
  
  `$ rosrun rosserial_arduino make_libraries.py` 
  
  To confirm everything was set up correctly, type: 
  
  `$ cd <sketchbook>/libraries/ros_lib/examples/`
  
  `$ ls`
  
  The contents should be similar to the following files: 
  
  ADC     button_example  IrRanger  pubsub       ServoControl  Ultrasound Blink    Clapper          Logging   ServiceClient  Temperature BlinkM  HelloWorld      Odom     ServiceServer  TimeTF

To start the Arduino IDE, type: 

`$ arduino` 

To be able to access the serial ports on the Ubuntu machine, you have to add yourself to the dialout groups. This is done with

`$ sudo username -a -G dialout <username>`

You should now be good to go to start using the Arduino IDE with ROS. Navigate to the toolbar and click on File -> Examples -> ros_lib to see the ROS examples that are provided with the library we previously set up. Tutorials on how to use these can be found here: http://wiki.ros.org/rosserial_arduino/Tutorials 

### Servo Control with rosserial and Arduino

To use the servo control functionality, first open the servo control example from the Arduino IDE examples -> ros_lib section. 

The key to the Arduino script that appears is that there is a global Servo object, it is attached to pin 9 of the Arduino board (must be a PWM pin), and then on each servo callback loop, a new angle is written to the servo. 

The only major difference between this sketch and sketches not using ROS is the instance of a node handler, nh. You can read more about node handelers here: http://wiki.ros.org/rosserial_arduino/Tutorials/NodeHandle%20and%20ArduinoHardware 

To test the servo control code, you will need to run a roscore and rosserial python node in their own terminals. 

In one terminal, type: 

`$ roscore`

Open a new terminal and type: 

`$ rosrun rosserial_python serial_node.py _port:=/dev/ttyACM0`

In a third terminal window we can publish angles to the arduino with rostopic pub. 

`$rostopic pub servo std_msgs/UInt16 <angle>`
