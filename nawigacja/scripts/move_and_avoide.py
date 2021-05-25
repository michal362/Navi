#!/usr/bin/env python

import rospy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

dist_to_obstacles = 1

def callback(msg):
    rospy.loginfo(rospy.get_caller_id() + 'The distance to obstacle is - %s', msg.ranges[0]) #print on terminal
    
    if msg.ranges[0] > dist_to_obstacles:
        print("move forward")
        move.linear.x = 0.2
        move.angular.z = 0.0
    if msg.ranges[0] <= dist_to_obstacles:
        print("turn left")
        move.linear.x = 0
        move.angular.z = 0.3
    
    pub.publish(move)

rospy.init_node('sub_node',anonymous=True)
sub = rospy.Subscriber('/scan', LaserScan, callback) # subscribe data from LaserScan topic
pub = rospy.Publisher('/cmd_vel', Twist, queue_size=2)
rate = rospy.Rate(2)
move = Twist()

rospy.spin()