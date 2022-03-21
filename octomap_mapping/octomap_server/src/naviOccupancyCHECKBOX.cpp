#include <ros/ros.h>
#include <nav_msgs/MapMetaData.h>
#include <nav_msgs/OccupancyGrid.h>
#include "std_msgs/Header.h"
#include "nav_msgs/MapMetaData.h"
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sstream>

double global_x;
double global_y;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
move_base_msgs::MoveBaseGoal goal;
geometry_msgs::Twist rot;
int un;


class Point2D
{
  private:
    double x,y;
  public:
    Point2D(){x=0;y=0;}
    void setX(double a)
    {
      x = a;
    }
    void setY(double b)
    {
      y = b;
    }
    double getX(void)
    {
      return x;
    }
    double getY(void)
    {
      return y;
    }
};

void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
  bool found_point = true;
  bool end_mapping = true;
  double occupied = 0;
  double free = 0;
  double unknown = 0;
  double unknownCount = 0;
  double obstacle = 0;
  Point2D* min = new Point2D();
  Point2D* max = new Point2D();
  
  

  std_msgs::Header header = msg->header;
  nav_msgs::MapMetaData info = msg->info;
  //std::cout<<"resolution: "<<info.resolution<<std::endl;
  //::cout<<"info_height: "<<info.height<<std::endl;
  //std::cout<<"info_width: "<<info.width<<std::endl;
  //std::cout<<"Origin_(0,0): "<<info.origin<<std::endl;


  std::vector<int8_t, std::allocator<int8_t>> data = msg->data;
  for(double x = 0; x<info.width; x++)
  {
    for(double y = 0; y<info.height; y++)
    {
      if((msg->data[x+ info.width*y]>70))
      {
        occupied++;
      }
      else if((msg->data[x+ info.width*y]==0))
      {
        free++;
        min->setX(x - 5);min->setY(y - 5);
        max->setX(x + 5);max->setY(y + 5);
        for(double ix = min->getX();ix <max->getX();ix+=1)
        {
            for(double iy = min->getY();iy <max->getY();iy+=1)
            {
                if((msg->data[ix+ info.width*iy]==-1))
                {
                  unknownCount++;
                }
                else if((msg->data[ix+ info.width*iy]>70))
                {
                  obstacle++;
                }
            }
        }
        if(found_point == true && unknownCount>un && obstacle == 0)
        {
            found_point=false;
            end_mapping = false;
            global_x = x*0.05-10.525;
            global_y = y*0.05-10.525;
            std::cout<<"x : "<<global_x<<std::endl;
            std::cout<<"y : "<<global_y<<std::endl;
            MoveBaseClient ac("move_base", true);
            ac.waitForServer();
            goal.target_pose.header.frame_id = "map";
            goal.target_pose.header.stamp = ros::Time::now();
            goal.target_pose.pose.position.x = global_x;
            goal.target_pose.pose.position.y = global_y;
            goal.target_pose.pose.orientation.w = 1.0;
            std::cout<<"Sending goal..."<<std::endl;
            ac.sendGoal(goal);
            std::cout<<"Driving to goal: x: "<<global_x<<" y: "<<global_y<<std::endl;
            std::cout<<"Unknown: "<<unknownCount<<std::endl;
            ac.waitForResult();

            if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            {
              std::cout<<"Goal reached!"<<std::endl;
              std::cout<<"--------------------------------------"<<std::endl;
            }
            else
            {
              std::cout<<"Goal not reached for some reason"<<std::endl;
            }
        }
        unknownCount=0;
        obstacle=0;
      }
      else if((msg->data[x+ info.width*y]==-1))
      {
        unknown++;
      }
    }
  }
  std::cout<<"Number of occupied cells is: "<<occupied<<std::endl;
  std::cout<<"Number of free cells is: "<<free<<std::endl;
  std::cout<<"Number of unknown cells is: "<<unknown<<std::endl;
  delete min,max;
  if(end_mapping)
  {
    ROS_INFO("END OF MAPPING");
    system("rosservice call /rtabmap/pause");
    system("rosservice call /rtabmap/get_map");
  }
}
int main(int argc, char **argv){
  std::cout<<"choose unkown"<<std::endl;
  std::cin>>un;
  ros::init(argc, argv, "map_reader");
  ros::NodeHandle n;

  ros::Subscriber map_sub = n.subscribe("/map",1,mapCallback);
  

  ros::spin();
  return 0;
}