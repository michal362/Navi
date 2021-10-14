#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Point.h>
move_base_msgs::MoveBaseGoal goal;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
void callback(const geometry_msgs::Point& cord)
{
  double point_x = cord.x;
  double point_y = cord.y;
  MoveBaseClient ac("move_base", true);
  ac.waitForServer();
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = point_x;
  goal.target_pose.pose.position.y = point_y;
  goal.target_pose.pose.orientation.w = 1.0;

  std::cout<<"Sending goal..."<<std::endl;
  ac.sendGoal(goal);
  std::cout<<"Driving to goal: x: "<<point_x<<" y: "<<point_y<<std::endl;
  ac.waitForResult();
  
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    std::cout<<"Goal reached!"<<std::endl;
  }
  else
  {
    std::cout<<"Goal not reached for some reason"<<std::endl;
  }
}
int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("goal_xyz",1,callback);
  ros::spin();
  return 0;
}