#!/usr/bin/env python
import message_filters
import sys
sys.path.append('./')

import rospy
from nav_msgs.msg import Odometry
from slave_msgs.msg import robot_state
from nlink.msg import node_frame2
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Twist
from gazebo_msgs.msg import ModelState
from lib.ekf_localization import ekf_odom_uwb
import numpy as np
from math import sin, cos


# robot state: x, y, theta  (theta in robot_ekf_state.pose.pose.position.z)

pub_robot_state = rospy.Publisher('robot_state', robot_state, queue_size=10)
seq = 0
robot_name = "agent1"
list_info_sub = []

mut_1 = np.array([[0], [0], [0]])
sigmat_1 = np.ones((3, 3))
sampletime = 0.05
sigmaQ = np.array([[0.01, 0], [0, 0.01]])
alpha =  [0.1, 0.1, 0.1, 0.1, 0.2, 0.2]
# def quarter2angle(quaternion):
#     x = quaternion.x
#     y = quaternion.y
#     z = quaternion.z
#     w = quaternion.w

#     r = math.atan2(2*(w*x+y*z),1-2*(x*x+y*y))
#     p = math.asin(2*(w*y-z*x))
#     y = math.atan2(2*(w*z+x*y),1-2*(z*z+y*y))

def robot_msg_callback(*args):
    #  args[0] -- uwb
    #  args[1] -- odom   

    model_state = ModelState()
    robot_state_msg = robot_state()

    global seq, mut_1, sigmat_1

    # global num_robot
    seq += 1

    # header
    robot_state_msg.header.stamp = rospy.Time.now()
    robot_state_msg.header.seq = seq
    robot_state_msg.header.frame_id = "global"

    # name
    model_state.model_name = robot_name

    # pose ekf fusion from uwb and odom 

    ut = np.array([[args[1].twist.twist.linear.x], [args[1].twist.twist.angular.z]])
    zt = np.array([[args[0].position.x], [args[0].position.y]])

    mu_t, sigma_t = ekf_odom_uwb(mut_1, sigmat_1, ut, zt, alpha, sigmaQ, sampletime)
    
    pose = Pose()

    pose.position.x = mu_t[0, 0]    
    pose.position.y = mu_t[1, 0] 
    pose.position.z = mu_t[2, 0] 

    model_state.pose = pose

    # twist
    linear = args[1].twist.twist.linear.x

    model_state.twist.linear.x = linear * cos(mu_t[2, 0])
    model_state.twist.linear.y = linear * sin(mu_t[2, 0]) 
    model_state.twist.angular = args[1].twist.twist.angular

    model_state.reference_frame = "global"

    mut_1 = mu_t
    sigmat_1 = sigma_t
    # robot_state_msg
    robot_state_msg.modelstate = model_state

    pub_robot_state.publish(robot_state_msg)

    print("robot2trans: publish robot information messages successfully")

def robot_state_publish():
    
    rospy.init_node('robot2trans', anonymous=True)
    rate=rospy.Rate(50)
    # should input the robot name from commond line
    global robot_name 

    if (len(sys.argv)<2):
        rospy.loginfo_once("no input, using the default robot name: agent1")
    else:
        robot_name = sys.argv[1]
            
    topic_name_odom = "/" + robot_name+"/odom"
    topic_name_uwb =  "/" + robot_name+"/nlink_linktrack_nodeframe2"   
    list_info_sub.append(message_filters.Subscriber(topic_name_uwb, node_frame2))
    list_info_sub.append(message_filters.Subscriber(topic_name_odom, Odometry))

    ts_info = message_filters.ApproximateTimeSynchronizer(list_info_sub, 10, 1)
    ts_info.registerCallback(robot_msg_callback)

    print("Configure successfully, waiting for odom and uwn message")

    rospy.spin()


    while not rospy.is_shutdown():
        rate.sleep()
        
if __name__=='__main__':
    try:
        robot_state_publish()
    except rospy.ROSInterruptException:
        pass



