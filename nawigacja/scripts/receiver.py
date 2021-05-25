#!/usr/bin/env python
import rospy
from std_msgs.msg import String 

def subscriberCallback(data):
    rospy.loginfo(rospy.get_caller_id() + "I received -- %s", data.data)#print on terminal

def listener():
    rospy.init_node('subscribeNode',anonymous=True)
    rospy.Subscriber('talker', String, subscriberCallback)
    rospy.spin() # the python file does not exit


if __name__=='__main__':
    listener()