#!/usr/bin/env python
import ros
import os
import rospy
from geometry_msgs.msg import Twist,Pose
from nav_msgs.msg import Odometry
from sensor_msgs.msg import LaserScan


global flaga
flaga = 1

regions_ = {
    'right': 0,
    'fright': 0,
    'front': 0,
    'fleft': 0,
    'left': 0,
}
state_ = 0
state_dict_ = {
    0: 'find the wall',
    1: 'turn left',
    2: 'follow the wall',
}


def position(pose):
    global pos_x,pos_y,start_x,start_y,flaga,t
    pos_x = round(pose.pose.pose.position.x,2)
    pos_y = round(pose.pose.pose.position.y,2)
    now = rospy.get_rostime()
    t_actual = now.secs


#if(flaga and state_ == 2):
    if(flaga and state_ == 2):
        start_x = pos_x
        start_y = pos_y
        t = now.secs
        flaga = 0
    
    print("X position: {}".format(pos_x))
    print("Y position: {}".format(pos_y))
    if(flaga == 0 and (t_actual>t+15.0)):
        print("Start point x:{} , y:{}".format(start_x,start_y))
        if(check(start_x,pos_x) and check(start_y,pos_y)):
            stop()
            #os.system("rosservice call /rtabmap/set_mode_localization")
            rospy.signal_shutdown(reason="Mapping finished")
    print("--------------------------------------------")

def check(value1,value2):
    if value1 - 0.3 <= value2 <= value1 + 0.3:
        return True

    
def laser_values(msg):
    global regions_
    regions_ = {
        'right':  min(msg.ranges[270:314]),
        'fright': min(msg.ranges[315:359]),
        'front':  min(msg.ranges[344:359]),
        'fleft':  min(msg.ranges[0:45]),
        'left':   min(msg.ranges[46:90]),
        
    }

    take_action()

def change_state(state):
    global state_, state_dict_
    if state is not state_:
        print ('Wall follower - [%s] - %s' % (state, state_dict_[state]))
        state_ = state

def take_action():
    global regions_
    regions = regions_
    msg = Twist()
    linear_x = 0
    angular_z = 0
    
    state_description = ''
    
    d = 1
    
    if regions['front'] > d and regions['fleft'] > d and regions['fright'] > d:
        state_description = 'case 1 - nothing'
        change_state(0)
    elif regions['front'] < d and regions['fleft'] > d and regions['fright'] > d:
        state_description = 'case 2 - front'
        change_state(1)
    elif regions['front'] > d and regions['fleft'] > d and regions['fright'] < d:
        state_description = 'case 3 - fright'
        change_state(2)
    elif regions['front'] > d and regions['fleft'] < d and regions['fright'] > d:
        state_description = 'case 4 - fleft'
        change_state(0)
    elif regions['front'] < d and regions['fleft'] > d and regions['fright'] < d:
        state_description = 'case 5 - front and fright'
        change_state(1)
    elif regions['front'] < d and regions['fleft'] < d and regions['fright'] > d:
        state_description = 'case 6 - front and fleft'
        change_state(1)
    elif regions['front'] < d and regions['fleft'] < d and regions['fright'] < d:
        state_description = 'case 7 - front and fleft and fright'
        change_state(1)
    elif regions['front'] > d and regions['fleft'] < d and regions['fright'] < d:
        state_description = 'case 8 - fleft and fright'
        change_state(0)
    else:
        state_description = 'unknown case'
        rospy.loginfo(regions)

def find_wall():
    msg = Twist()
    msg.linear.x = 0.2
    msg.angular.z = -0.3
    return msg

def turn_left():
    msg = Twist()
    msg.angular.z = 0.3
    return msg

def follow_the_wall():
    global regions_
    
    msg = Twist()
    msg.linear.x = 0.3
    return msg
def stop():
    msg = Twist()
    msg.linear.x = 0
    msg.angular.z = 0
    pub_.publish(msg)
    
    

rospy.on_shutdown(stop)

def main():
    global pub_,sub
    
    rospy.init_node('wall_follower',anonymous=True)

    
    pub_ = rospy.Publisher('/cmd_vel', Twist, queue_size=1)
    
    sub = rospy.Subscriber('/scan', LaserScan, laser_values)

    sub_pose = rospy.Subscriber('/odom',Odometry,position)
    pose = Pose()
    rate = rospy.Rate(20)
    while not rospy.is_shutdown():
        msg = Twist()
        if state_ == 0:
            msg = find_wall()
        elif state_ == 1:
            msg = turn_left()
        elif state_ == 2:
            msg = follow_the_wall()
            pass
        else:
            rospy.logerr('Unknown state!')
        
        
        pub_.publish(msg)
        
        rate.sleep()

if __name__ == '__main__':
    main()