#!/usr/bin/env python

#Laser ranges = 360

import rospy
from sensor_msgs.msg import LaserScan

#Sprawdzenie czy dane z lasera sa odczytywane
def check_lasser(msg):

    rospy.loginfo('Checking lasser ...')
    while msg is None and not rospy.is_shutdown():
        try:
            msg = rospy.wait_for_message('/sacn', LaserScan,timeout=1.0)
            rospy.logdebug("Current /scan is READY" + str(msg))
        except:
            rospy.logerr("Current /scan is not READY yet, retrying for getting scan")
    rospy.loginfo("checking laser... DONE")
    return msg

def laser_values_test(msg):
    check_lasser(msg)

    print('laser ranges: {}'.format(len(msg.ranges)))
    print('laser value of 0 degree: {}'.format(msg.ranges[0])) # front
    print('laser value of 45 degree: {}'.format(msg.ranges[45])) # 45 degrees
    print('laser value of 90 degree: {}'.format(msg.ranges[90])) # lewo
    print('laser value of 135 degree: {}'.format(msg.ranges[135])) # 135 degrees
    print('laser value of 180 degree: {}'.format(msg.ranges[180])) # tyl
    print('laser value of 225 degree: {}'.format(msg.ranges[225])) # 225 degrees
    print('laser value of 270 degree: {}'.format(msg.ranges[270])) # prawo
    print('laser value of 315 degree: {}'.format(msg.ranges[315])) # 315 degrees


rospy.init_node('scan_values',anonymous=True)
sub = rospy.Subscriber('/scan',LaserScan,laser_values_test)
rospy.spin()