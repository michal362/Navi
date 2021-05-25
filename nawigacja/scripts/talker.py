#!/usr/bin/env python

import rospy
from std_msgs.msg import String

def publishMethod():
    pub = rospy.Publisher('talker', String, queue_size=10)
    rospy.init_node('publish_node', anonymous=True)
    rate = rospy.Rate(10) #10Hz
    while not rospy.is_shutdown():
        pyblishString = 'This is being published' #varaible
        rospy.loginfo("Data is being sent") #print on terminal
        pub.publish(pyblishString) 
        rate.sleep()

if __name__=='__main__':
    try:
        publishMethod()
    except rospy.ROSInternalException:
        pass
