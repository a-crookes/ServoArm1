#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/UInt16MultiArray.h"
#include "sensor_msgs/JointState.h"

#define PI 3.14159

// Umrechnung Radian->Degree
// pi=180°
// aRadien/pi*180

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
uint _DataArray[7];

/**
  First convert each angle from the position array to the center angle by adding either 1.5708 rad or 3.14159 radians depending if the servo is a 180 deg. servo or 400 deg servo

  There is some slop in the servos due to using cheap hobby servos with non-precise motor hubs attached to them, therefore the slop is subtracted off

*/

double centerArray[7] = {180, 90, 90, 90, 90, 90, 90};
double slopArray[7] = {0,-.1,.09,0,.05,0,0};
double ratio[7] = {1, 4.2105, 8, 16, 2, 2, 1};

void chatter_cb(const sensor_msgs::JointState::ConstPtr& msg)
{
  int i=0;
  _DataArray[0]= centerArray[0]+uint(((msg->position[0])/PI*180)/(ratio[0]));
  _DataArray[1]= centerArray[1]-uint(((msg->position[1])/PI*180)/(ratio[1])); //16/3.8: gearbox ratio
  _DataArray[2]= centerArray[2]+uint(((msg->position[2])/PI*180)/(ratio[2])); //2:1 gearbox ratio
  _DataArray[3]= centerArray[3]-uint(((msg->position[3])/PI*180)/(ratio[3]));
  _DataArray[4]= centerArray[4]-uint(((msg->position[4])/PI*180)/(ratio[4]));
  _DataArray[5]= centerArray[5]+uint(((msg->position[5])/PI*180)/(ratio[5]));
  _DataArray[6]= centerArray[6]+uint(((msg->position[6])/PI*180)/(ratio[6]));
ROS_INFO_NAMED("test", "joint1 position: %d", _DataArray[0]);
}




int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "parse_and_publish");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("joint_states", 7, chatter_cb);

  ros::Publisher pub = n.advertise<std_msgs::UInt16MultiArray>("joint_array", 7);
  
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    std_msgs::UInt16MultiArray array;
    //Clear array
    array.data.clear();

    //for loop, pushing data in the size of the array
    for (int i = 0; i < 7; i++)
    {
     //array[i]= _DataArray[i];
     array.data.push_back(_DataArray[i]);
    }
    pub.publish(array);

    ros::spinOnce();

    loop_rate.sleep();
  }
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  

  return 0;
}
