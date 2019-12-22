#include "ros/ros.h"
#include <string>
#include <math.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <gazebo_msgs/WorldState.h>

float cal_yaw(geometry_msgs::Quaternion quater);
int cout_flag = 0;
float angular_max = 1.2;
float pi = 3.1415926;

std::string robot_name;

geometry_msgs::Vector3  msg_robot_linear;
geometry_msgs::Vector3  msg_robot_angular;

geometry_msgs::Twist msg;

ros::Publisher chatter_pub;


void robotControl_callback(const gazebo_msgs::WorldState::ConstPtr &msgInput)
{

  int msg_index = 0;

  geometry_msgs::Twist msg_twist;

  for(int ind = 0; ind < msgInput->name.size(); ind++){
    if(robot_name.compare(msgInput->name[ind])==0){
      msg_index = ind;
    }
  }
  msg_twist = msgInput->twist[msg_index];
  
    geometry_msgs::Twist pub_twist;
    float rvo_x = msg_twist.linear.x;
    float rvo_y = msg_twist.linear.y;

    if (rvo_x == 0.0f && rvo_y == 0.0f)
    {
        pub_twist.linear.x = 0;
        pub_twist.angular.z = 0;
    }
    else
    {
      double speed = sqrt(pow(rvo_x, 2) + pow(rvo_y, 2));

      if (speed < 0.01)
          pub_twist.linear.x = 0;
      else
          pub_twist.linear.x = speed;

      float angle_yaw = cal_yaw(msgInput->pose[msg_index].orientation);
      float angle_vel = atan2(rvo_y, rvo_x);
      
      float diff = angle_yaw - angle_vel;

      // twist.angular.z = (diff > 0.02) ? (diff < 3.1415926 ? -0.5 : 0.5) : (diff < );  
      // float angular_vel = fabsf(angular_max * float(diff/pi));

      if (diff > 0.1)
          pub_twist.angular.z = diff < pi ? - angular_max : angular_max;
      else if (diff < -0.1)
          pub_twist.angular.z = diff < -pi ? -angular_max : angular_max;
      else
          pub_twist.angular.z = 0;

    }
  ROS_INFO("trans2robot: new velocity to %s is %f %f",robot_name.c_str(),pub_twist.linear.x,pub_twist.angular.z);
  chatter_pub.publish(pub_twist);
  ROS_INFO("trans2robot: publish new velocity to robot successfully");
}

float cal_yaw(geometry_msgs::Quaternion quater)
{
    float x = quater.x;
    float y = quater.y;
    float z = quater.z;
    float w = quater.w;

    float raw = std::atan2(2 * (w * z + x * y), 1 - 2 * (pow(z, 2) + pow(y, 2)));
    return raw;
}

// input robot id in argv
int main(int argc, char **argv)
{  
  ros::init(argc, argv, "trans2robot");
  ros::NodeHandle n;
  chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop", 100);

  robot_name = argv[1];

  ros::Subscriber sub=n.subscribe("/global/multi_vel", 100, robotControl_callback);
	
  ros::Rate loop_rate(50);

  while (ros::ok())
  {
    
    //ROS_INFO("Published");
    ros::spinOnce();
 
    loop_rate.sleep();

  }
  return 0;
}

