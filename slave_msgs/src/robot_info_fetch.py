# # !/usr/bin/env python
# import message_filters
# import sys
# import rospy
# from sensor_msgs.msg import Imu
# from nav_msgs.msg import Odometry
# from slave_msgs.msg import robot_info
#
# info = robot_info()
# seq = 0
#
# def info_callback(odom, imu):
#     # time_step1
#     global seq
#     id = int(sys.argv[1])
#     seq += 1
#
#     # header
#     info.header.stamp = rospy.Time.now()
#     info.header.seq = seq
#     info.header.frame_id = "robot" + str(id)
#
#     # name and id
#     info.robot_name = "agent" + str(id)
#     info.robot_id = id
#
#     # pose
#     info.pose = odom.PoseWithCovariance
#
#     # twist
#     print("receive multiple odometry messages successfully")
#
# def information_fetch():
#
#     rospy.init_node('robot_info_fetch', anonymous=True)
#     topic_name = '/robot_info'
#
#     pub_info = rospy.Publisher(topic_name, robot_info, queue_size=10)
#
#     if len(sys.argv) < 2:
#         rospy.logerr_once("Please input the id of robot")
#     else:
#         odom = message_filters.Subscriber("/Slave01/odom", Odometry)
#         imu = message_filters.Subscriber("/Slave01/mobile_base/sensors/imu_data", Imu)
#         ts_info = message_filters.ApproximateTimeSynchronizer([odom, imu], 10, 0.05)
#         ts_info.registerCallback(info_callback)
#         rospy.spin()
#
# if __name__=='__main__':
#     try:
#         information_fetch()
#     except rospy.ROSInterruptException:
#         pass
#
#
#
#
#
#
#
#
