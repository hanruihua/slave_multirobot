#!/usr/bin/env python
# license removed for brevity
import rospy
import numpy as np 
import matplotlib.pyplot as plt
from std_msgs.msg import String
from geometry_msgs.msg import PointStamped
from slave_msgs.msg import uwb_position

vo_arr_x =  [];
vo_arr_y =  [];
uwb_arr_x = [];
uwb_arr_y = [];

def vo_callback(data):
    vox = round(data.point.x + 1, 2)
    voy = round(data.point.y + 1, 2)
    voz = round(data.point.z + 1, 2)
    plt.plot(vox, voy, 'ro')
    plt.draw()
    plt.pause(0.00000000001)
    rospy.loginfo("vo message received x %lf y %lf z %lf ", vox, voy, voz)
    #vo_arr_x.append(voy)
    #vo_arr_y.append(voz)
def uwb_callback(data):
    rospy.loginfo("uwb message received x %lf y %lf z %lf ", data.position.x, data.position.y, data.position.z)
    uwb_arr_x.append(data.position.x)
    uwb_arr_y.append(data.position.y)
        

def listener():
    rospy.init_node('plot', anonymous=True)
    rospy.Subscriber("vo_position", PointStamped, vo_callback)
    rospy.Subscriber("uwb_position", uwb_position, uwb_callback)
    plt.ion()
    plt.show()
    rospy.spin()

if __name__ == '__main__':
    listener()
