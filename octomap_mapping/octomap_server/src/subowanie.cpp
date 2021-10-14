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
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
using octomap_msgs::GetOctomap;
using namespace std;
using namespace octomap;
using namespace octomap_msgs;

double global_x;
double global_y;
double global_check_z = 0;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
geometry_msgs::Twist rot;

move_base_msgs::MoveBaseGoal goal;
class Mapa
{
    public:
        ros::NodeHandle nh;
        ros::Publisher pub_rotate;

        void ChatterCallback(const octomap_msgs::Octomap &msg);

    Mapa()
    {
        pub_rotate = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);
    };
};
void Mapa::ChatterCallback(const octomap_msgs::Octomap &msg)
{
    std::cout<<"processing..."<<endl;
    
    int free = 0;
    int left = 0;
    int unknown = 0;
    int unknownV2 = 0;
    int occupied = 0;
    int flaga = 0;
    bool found_point = true;
    point3d min;
    point3d max;
    AbstractOcTree* tree = octomap_msgs::msgToMap(msg);

    if(tree)
    {
       OcTree* octree = dynamic_cast<OcTree*>(tree);

       if(octree)
       {
           std::cout<<"Map received..."<<endl;
            
            for(OcTree::leaf_iterator it = octree->begin_leafs(),end = octree->end_leafs(); it!= end; ++it)
            {
                
                if(octree->isNodeOccupied(*it))
                {
                    double i_x = it.getX();
                    double i_y = it.getY();
                    double i_z = it.getZ();
                    if(fabs(i_z - 0.025)<0.0001)
                    {
                        occupied++;
                    }
                    min.x() = i_x - 0.25;min.y() = i_y - 0.25;min.z() = 0.025;
                    max.x() = i_x + 0.25;max.y() = i_y + 0.25;max.z() = 0.026;

                    for(double ix = min.x();ix <max.x();ix+=0.05)
                    {

                        for(double iy = min.y();iy <max.y();iy+=0.05)
                        {
                            for(double iz = min.z();iz <max.z();iz+=0.025)
                            {
                                if(!octree->search(ix,iy,iz))
                                {
                                    unknown++;
                                    unknownV2++;
                                }
                                if (found_point == false) break;
                            }
                            if(octree->search(ix,iy,0.05) != NULL)
                            {
                                if(octree->isNodeOccupied(octree->search(ix,iy,0.05)))
                                {
                                    global_check_z++;
                                }
                            }
                        }
                    }




                    if(found_point == true && unknown>30 && global_check_z == 0)
                    {
                        found_point == false;
                        global_x = i_x;
                        global_y = i_y;
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
                        std::cout<<"Unknown: "<<unknown<<std::endl;
                        ac.waitForResult();

                        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                        {
                            std::cout<<"Goal reached!"<<std::endl;
                            std::cout<<"--------------------------------------"<<std::endl;
                            rot.angular.z = 0.5;
                            pub_rotate.publish(rot);
                            sleep(14);
                            rot.angular.z = 0;
                            pub_rotate.publish(rot);
                            break;
                        }
                        else
                        {
                            std::cout<<"Goal not reached for some reason"<<std::endl;
                        }
                    }

                    
                    unknown = 0;
                    global_check_z = 0;
    
                }
                else
                {
                    
                    free++;
                    
                }
                
            }
            std::cout<<"Number of occupied cells = "<<occupied<<endl;
            std::cout<<"Number of Free cells: "<<free<<endl;


            cout<<"global_x: "<<global_x<<endl;
            cout<<"global_y: "<<global_y<<endl;  
        } 

    }

    delete tree;
}



int main (int argc, char **argv)
{
    ros::init(argc,argv, "sub3");
    ros::NodeHandle n,nh;
    Mapa map;
    ros::Subscriber sub = n.subscribe("/octomap_binary",1,&Mapa::ChatterCallback,&map);
    
    while (ros::ok())
    {

        ros::spinOnce();
 
    }
    return 0;
}