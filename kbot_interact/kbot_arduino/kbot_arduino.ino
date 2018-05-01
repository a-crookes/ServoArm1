
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt16MultiArray.h>
#include <Servo.h>
#include <AccelStepper.h>

ros::NodeHandle  nh;

//Joint 1 - proximal arm roll
#define stepPin        30
#define dirPin         28

AccelStepper joint1(AccelStepper::DRIVER,stepPin,dirPin);

Servo joint2;
Servo joint3;
Servo joint4;
Servo joint5;
Servo joint6;
Servo gripper_r;
Servo gripper_l;

unsigned int _joint1Angle = 180;
unsigned int _joint2Angle = 90;
unsigned int _joint3Angle = 90;
unsigned int _joint4Angle = 90;
unsigned int _joint5Angle = 90;
unsigned int _joint6Angle = 90;
unsigned int _gripper_rAngle =73; //closed

void kbot_cb(const std_msgs::UInt16MultiArray& angleArray){
  _joint1Angle = (unsigned int)angleArray.data[0];
  _joint2Angle = (unsigned int)angleArray.data[1];
  _joint3Angle = (unsigned int)angleArray.data[2];
  _joint4Angle = (unsigned int) angleArray.data[3];
  _joint5Angle = (unsigned int)angleArray.data[4];
  _joint6Angle = (unsigned int)angleArray.data[5];
  _gripper_rAngle = (unsigned int)angleArray.data[6];
}
void kbot_move(unsigned int stepDelay,unsigned int vJoint1, unsigned int vJoint2, unsigned int vJoint3, unsigned int vJoint4, unsigned int vJoint5, unsigned int vJoint6, unsigned int vGripper_r) {
        
  //check safety angles
  if (stepDelay > 30) stepDelay = 30;
  if (stepDelay < 10) stepDelay = 10;
  if (vJoint2 < 70) vJoint2= 70;
  if (vJoint2 > 110) vJoint2=110;
  if (vJoint3 < 74) vJoint3=74;
  if (vJoint3 > 105) vJoint3=105;
  if (vJoint4 < 70) vJoint4=70;
  if (vJoint4 > 110) vJoint4=110;
  if (vJoint5 < 0) vJoint5=0;
  if (vJoint5 > 180) vJoint5=180;
  if (vJoint6 > 180) vJoint6=180;
  if (vJoint6 < 0) vJoint6=0;
  if (vGripper_r < 10) vGripper_r = 10;
  if (vGripper_r > 73) vGripper_r = 73;

  joint1.moveTo(vJoint1*10);
  joint2.write(vJoint2);
  joint3.write(vJoint3);
  joint4.write(vJoint4);
  joint5.write(vJoint5);
  joint6.write(vJoint6);
  gripper_r.write(vGripper_r);
  gripper_l.write(vGripper_r-180);

  //joint1.run();
}

ros::Subscriber<std_msgs::UInt16MultiArray> sub("joint_array", &kbot_cb);

void setup()
{  
  nh.initNode();
  nh.subscribe(sub);
  joint2.attach(2);
  joint3.attach(3);
  joint4.attach(4);
  joint5.attach(6);
  joint6.attach(7);
  gripper_r.attach(9);
  gripper_l.attach(10);

  joint1.setMaxSpeed(2000);
  joint1.setAcceleration(1000);
}

void loop()
{ 
  kbot_move(20,_joint1Angle,_joint2Angle,_joint3Angle,_joint4Angle,_joint5Angle,_joint6Angle,_gripper_rAngle);
  joint1.run();
  nh.spinOnce();
  delay(1);
}

