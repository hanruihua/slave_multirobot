# !/usr/bin/env python
import message_filters
import sys
import rospy
from sensor_msgs.msg import Imu 
from nav_msgs.msg import Odometry
from slave_msgs.msgs import robot_info

def info_callback(odo_data1, odo_data2):
    # time_step1
    print("receive multiple odometry messages successfully")

def robot_info():

    rospy.init_node('robot_info_fetch', anonymous=True)
    robot_id = sys.argv[1]
    pub_info = rospy.Publisher('robot', robot_info, queue_size=10)

    list_odo_sub = []
    list_imu_sub = []

    if len(sys.argv) < 2:
        rospy.logerr_once("Please input the number of the slave messages")
    else:
        # num_message = sys.argv[1]
        # for i in range(int(num_message)):
        #     topic_name_odom = "Slave0"+str(i+1)+"/odom"
        #     topic_name_imu = "Slave0"+str(i+1)+"/mobile_base/sensors/imu_data"
        #
        #     list_odo_sub.append(message_filters.Subscriber(topic_name_odom, Odometry))
        #     list_imu_sub.append(message_filters.Subscriber(topic_name_imu, Imu))
        a = message_filters.Subscriber("/Slave01/odom", Odometry)
        b = message_filters.Subscriber("/Slave02/odom", Odometry)

        # c = message_filters.Subscriber("/Slave01/mobile_base/sensors/imu_data", Imu)
        # d = message_filters.Subscriber("/Slave02/mobile_base/sensors/imu_data", Imu)

        # ts_odom = message_filters.ApproximateTimeSynchronizer([a, b], 100, 0.5, allow_headerless=True)
        # ts_imu = message_filters.ApproximateTimeSynchronizer([c, d], 100, 0.5)

        ts_odom = message_filters.ApproximateTimeSynchronizer([a, b], 10, 0.1)
        # ts_imu = message_filters.TimeSynchronizer([c, d], 1000)

        ts_odom.registerCallback(odo_callback)
        # ts_imu.registerCallback(imu_callback)
        rate=rospy.Rate(10)
        rospy.spin()
        
    while not rospy.is_shutdown():
        rate.sleep()
        
if __name__=='__main__':
    try:
        robot_info()
    except rospy.ROSInterruptException:
        pass








