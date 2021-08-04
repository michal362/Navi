#!/usr/bin/env python
import rospy
import actionlib
from geometry_msgs.msg import Point
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

class Navi:
    def __init__(self):
        rospy.init_node('movebase_client_py')
        self.point_sub = rospy.Subscriber("goal_xyz",Point,self.target)

    def target(self,msg):
        self.x = msg.x
        self.y = msg.y
        print("x: {}".format(msg.x))
        print("y: {}".format(msg.y))

    def movebase_client(self):
        client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
        client.wait_for_server()

        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = self.x
        goal.target_pose.pose.position.y = self.y
        goal.target_pose.pose.orientation.w = 1.0

        client.send_goal(goal)
        wait = client.wait_for_result()
        if not wait:
            rospy.logerr("Action server not available!")
            rospy.signal_shutdown("Action server not available!")
        else:
            return client.get_result()

if __name__ == '__main__':
    try:
        nawigacja = Navi()
 
        result = nawigacja.movebase_client()
        if result:
            rospy.loginfo("Goal execution done!")
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")