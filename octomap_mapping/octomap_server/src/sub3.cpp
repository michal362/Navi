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
using octomap_msgs::GetOctomap;
using namespace std;
using namespace octomap;
using namespace octomap_msgs;

double global_x;
double global_y;
int occupied = 0;
class Mapa
{
    public:

        void ChatterCallback(const octomap_msgs::Octomap &msg);
};
void Mapa::ChatterCallback(const octomap_msgs::Octomap &msg)
{
    //int occupied = 0;
    int free = 0;
    int unknown = 0;
    int flaga = 0;
    bool check = true;
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


                    if(flaga == 0)
                    {
                        cout<<"x: "<<i_x<<endl;
                        cout<<"y: "<<i_y<<endl;
                    }
                    flaga = 1;
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
                                    if(check) unknown++;

                                }
                            }
                        }
                    }
                    if(check == true && unknown>50)
                    {
                        global_x = i_x;
                        global_y = i_y;
                        
                    }
                    check = false;



                    //std::cout<<" x = "<< it.getX()<<endl;
                    //std::cout<<" y = "<< it.getY()<<endl;
                    //std::cout<<" z = "<< it.getZ()<<endl;
                    //std::cout<<"---------------------------------------"<<endl;
                    occupied++;
                    
                    
                }
                else
                {
                    free++;
                    
                }


                
            }
            std::cout<<"Number of occupied cells = "<<occupied<<endl;
            std::cout<<"Number of Free cells: "<<free<<endl;
            std::cout<<"Number of unknown cells: "<<unknown<<endl;
            cout<<"global_x: "<<global_x<<endl;
            cout<<"global_y: "<<global_y<<endl;
       } 

    }

    delete tree;
}



int main (int argc, char **argv)
{
    Mapa map;
    ros::init(argc,argv, "sub3");
    ros::NodeHandle n,nh;
    ros::Subscriber sub = n.subscribe("/octomap_binary",1000,&Mapa::ChatterCallback,&map);
    ros::Publisher pub_goal = nh.advertise<geometry_msgs::Point>("goal_xyz",1000);
    geometry_msgs::Point msg;
    while (ros::ok())
    {
        msg.x = global_x;
        msg.y = global_y;
        //w celu sprawdzenia czy dane się odświeżają
        msg.z = occupied;
        pub_goal.publish(msg);
        ros::spinOnce();
    }
    return 0;
}