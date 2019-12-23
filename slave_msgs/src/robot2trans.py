#!/usr/bin/env python
import message_filters
import sys
import rospy
from nav_msgs.msg import Odometry
from slave_msgs.msg import robot_state
from slave_msgs.msg import node_frame2
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Twist
from gazebo_msgs.msg import ModelState

pub_robot_state = rospy.Publisher('/robot_state', robot_state, queue_size=10)
seq = 0
robot_name = "agent"

def robot_msg_callback(*args):

    model_state = ModelState()
    robot_state_msg = robot_state()

    global seq
    # global num_robot
    seq += 1

    # header
    robot_state_msg.header.stamp = rospy.Time.now()
    robot_state_msg.header.seq = seq
    robot_state_msg.header.frame_id = "global"

    # name
    model_state.model_name = robot_name

    # pose
    pose = Pose()
    pose.position = args[0].position     # from uwb
    pose.orientation = args[1].pose.pose.orientation  # from odom
    model_state.pose = pose

    # twist
    twist = args[1].twist.twist
    model_state.twist = twist
    model_state.reference_frame = "global"

    robot_state_msg.modelstate = model_state

    pub_info.publish(robot_state_msg)

    print("robot2trans: publish robot information messages successfully")

def robot_state_publish():
    rospy.init_node('robot2trans', anonymous=True)
    rate=rospy.Rate(50)
    # should input the robot name from commond line
    global robot_name = sys.argv[1]
    topic_name_odom = robot_name+"/odom"
    topic_name_uwb =  robot_name+"/nlink_linktrack_nodeframe2"   
    list_info_sub.append(message_filters.Subscriber(topic_name_uwb, node_frame2))
    list_info_sub.append(message_filters.Subscriber(topic_name_odom, Odometry))

    ts_info = message_filters.ApproximateTimeSynchronizer(list_info_sub, 10, 0.1)
    ts_info.registerCallback(robot_msg_callback)

    rospy.spin()

    while not rospy.is_shutdown():
        rate.sleep()
        
if __name__=='__main__':
    try:
        robot_state_publish()
    except rospy.ROSInterruptException:
        pass



