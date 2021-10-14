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
double global_check_z = 0;

class Mapa
{
    public:

        void ChatterCallback(const octomap_msgs::Octomap &msg);
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
                    if(flaga == 0)
                    {
                        cout<<"x: "<<i_x<<endl;
                        cout<<"y: "<<i_y<<endl;
                        cout<<"z: "<<i_z<<endl;
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




                    if(found_point == true && unknown>10 && global_check_z == 0)
                    {
                        found_point == false;
                        global_x = i_x;
                        global_y = i_y;
                        
                    }
                    //check = false;
                    unknown = 0;
                    global_check_z = 0;
                    

                    //std::cout<<" x = "<< it.getX()<<endl;
                    //std::cout<<" y = "<< it.getY()<<endl;
                    //std::cout<<" z = "<< it.getZ()<<endl;
                    //std::cout<<"---------------------------------------"<<endl;
                    //occupied++;
                    
                    
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
    Mapa map;
    ros::NodeHandle n,nh;
    ros::Subscriber sub = n.subscribe("/octomap_binary",1,&Mapa::ChatterCallback,&map);
    ros::Publisher pub_goal = nh.advertise<geometry_msgs::Point>("goal_xyz",1);
    geometry_msgs::Point msg;
    while (ros::ok())
    {
        msg.x = global_x;
        msg.y = global_y;
        
        //w celu sprawdzenia czy dane się odświeżają
        if(pub_goal.getNumSubscribers() > 0)
            pub_goal.publish(msg);
        ros::spinOnce();
 
    }
    return 0;
}