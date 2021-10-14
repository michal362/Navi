#!/usr/bin/env python
import rospy
import actionlib
from geometry_msgs.msg import Point
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import os

class Navi:
    def __init__(self):
        rospy.init_node('movebase_client_py')
        self.point_sub = rospy.Subscriber("goal_xyz",Point,self.target)
        self.flaga = 1
        self.point = Point()
        
    def target(self,msg):
        self.point = msg

    
    def stop_program(self):
        os.system("rosservice call /rtabmap/pause")
        os.system("rosrun octomap_server octomap_saver saved_map.bt")

    def movebase_client(self):
        while(not rospy.is_shutdown()):
            client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
            client.wait_for_server()
    
            goal = MoveBaseGoal()
            goal.target_pose.header.frame_id = "map"
            goal.target_pose.header.stamp = rospy.Time.now()
            goal.target_pose.pose.position.x = self.point.x
            goal.target_pose.pose.position.y = self.point.y
            goal.target_pose.pose.orientation.w = -1.0
            
            client.send_goal(goal)
            print("Driving to goal: x:{0}, y:{1}".format(goal.target_pose.pose.position.x,goal.target_pose.pose.position.y))
            if(client.wait_for_result()):
                print("goal reached!")
            else:
                print("goal NOT reached")
            client.cancel_all_goals()
            #if(self.point.x == goal.target_pose.pose.position.x and self.point.y == goal.target_pose.pose.position.y):
                #print("End of mapping!")
                #rospy.is_shutdown()
                #rospy.on_shutdown(self.stop_program())              

if __name__ == '__main__':
    nawigacja = Navi()
    nawigacja.movebase_client()
