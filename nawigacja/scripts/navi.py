#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist,Pose
from nav_msgs.msg import Odometry
from math import pow, atan2, sqrt    

class MoveRobot:

    def __init__(self):
        rospy.init_node('turtlebot_controller',anonymous=True)
        self.vel_pub = rospy.Publisher("/cmd_vel",Twist,queue_size=10)
        self.pose_sub = rospy.Subscriber("/odom",Odometry,self.update_pose)

        self.pose = Pose()
        self.rate = rospy.Rate(10)
    def update_pose(self,msg):
        self.pose = msg
        self.pose.pose.pose.position.x = round(self.pose.pose.pose.position.x,2)
        self.pose.pose.pose.position.y = round(self.pose.pose.pose.position.y,2)

    def euclidean_distance(self, goal_pose):
        return sqrt(pow((goal_pose.position.x - self.pose.pose.pose.position.x), 2) + pow((goal_pose.position.y - self.pose.pose.pose.position.y), 2))

    def linear_vel(self, goal_pose, constant=0.1):
        return constant * self.euclidean_distance(goal_pose)

    def steering_angle(self, goal_pose):
        return atan2(goal_pose.position.y - self.pose.pose.pose.position.y, goal_pose.position.x - self.pose.pose.pose.position.x)

    def angular_vel(self, goal_pose, constant=0.2):
        return constant * (self.steering_angle(goal_pose) - (self.pose.pose.pose.orientation.z))

    def move2goal(self):
        
        goal_pose = Pose()

        goal_pose.position.x = float(input("set your x goal: "))
        goal_pose.position.y = float(input("set your y goal: "))

        self.didistance_tolerance = input("Set your tolerance: ")

        vel_msg = Twist()
        print(self.pose.pose.pose.position.x)
        print(self.pose.pose.pose.orientation.z)
        print(atan2(goal_pose.position.y - self.pose.pose.pose.position.y, goal_pose.position.x - self.pose.pose.pose.position.x))
        while round(self.pose.pose.pose.orientation.z) != round(self.steering_angle(goal_pose)):
            vel_msg.linear.x = 0
            vel_msg.linear.y = 0
            vel_msg.angular.z = self.angular_vel(goal_pose)
            self.vel_pub.publish(vel_msg)
        print("done")

        while round(goal_pose.position.x) != round(self.pose.pose.pose.position.x):
            vel_msg.linear.x = self.linear_vel(goal_pose)
            vel_msg.linear.y = 0
            vel_msg.linear.z = 0

            self.vel_pub.publish(vel_msg)
            self.rate.sleep()
        print("we got it")

        vel_msg.linear.x = 0
        vel_msg.angular.z = 0
        self.vel_pub.publish(vel_msg)
        rospy.spin()

if __name__ == '__main__':
    try:
        x = MoveRobot()
        x.move2goal()
        
    except rospy.ROSInterruptException:
        pass