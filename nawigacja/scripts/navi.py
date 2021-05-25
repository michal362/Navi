#!/usr/bin/env python
import rospy
import time
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

def shutdown():
    print ("shutdown program")
    move.linear.x = 0.0
    move.angular.z = 0.0
    pub.publish(move)

def laser_values(msg):
    
    przod = msg.ranges[0]
    cztery_piec = msg.ranges[45]
    lewo = msg.ranges[90]
    sto_trzy_piec = msg.ranges[135]
    prawo = msg.ranges[270]
    trzysta_pietnas = msg.ranges[315]
    tyl = msg.ranges[180]
    dwiescie_dwa_piec = msg.ranges[225]
    

    if przod > 1 and trzysta_pietnas > 0.5:
        move.linear.x = 0.2
        move.angular.z = 0.0
    if przod <= 1:                
        move.linear.x = 0.0
        move.angular.z = 0.1

    
    pub.publish(move)
    



rospy.init_node('navi1',anonymous=True)
sub = rospy.Subscriber('/scan', LaserScan, laser_values,queue_size=2)
pub = rospy.Publisher('/cmd_vel', Twist, queue_size=2)
rate = rospy.Rate(2)
move = Twist()
rospy.spin()