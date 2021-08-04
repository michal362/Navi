#include <octomap_msgs/conversions.h>
#include <octomap/OcTree.h>
#include <octomap/octomap.h>
#include <octomap/OcTreeBase.h>
#include <octomap/AbstractOccupancyOcTree.h>
#include "std_msgs/String.h"
#include <octomap_msgs/GetOctomap.h>
#include <fstream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h> 
using octomap_msgs::GetOctomap;
using namespace std;
using namespace octomap;
using namespace octomap_msgs;

class Punkt
{
    public:
        double x;
        double y;
        void callback(const geometry_msgs::PointPtr &data);
};
void Punkt::callback(const geometry_msgs::PointPtr &data)
{
    cout<<"x: "<<data->x<<endl;
    x = data->x;
    y = data->y;
}


int main (int argc, char **argv)
{
    Punkt punkt;
    ros::init(argc,argv, "subowanie");
    ros::NodeHandle n;
    ros::Publisher pub_goal = n.advertise<geometry_msgs::Point>("goal_xyz",1000);
    ros::Subscriber sub = n.subscribe("goal_xyz",1000,&Punkt::callback,&punkt);
    geometry_msgs::Point msg;
    msg.x = 5;
    msg.y = 10;
    ros::Rate(10);
    while(ros::ok())
    {
        pub_goal.publish(msg);
        cout<<"x: "<<punkt.x<<endl;
        cout<<"y: "<<punkt.y<<endl;
        cout<<"------------------------"<<endl;
        punkt.x++;
        ros::spinOnce();
        
    }
    return 0;
}