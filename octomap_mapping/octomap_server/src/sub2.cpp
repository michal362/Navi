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
using octomap_msgs::GetOctomap;
using namespace std;
using namespace octomap;
using namespace octomap_msgs;




void ChatterCallback(const octomap_msgs::Octomap &msg)
{
    int occupied = 0;
    int free = 0;
    AbstractOcTree* tree = octomap_msgs::msgToMap(msg); 
    //AbstractOccupancyOcTree* octree = NULL; 
    
    if(tree)
    {
       OcTree* octree = dynamic_cast<OcTree*>(tree);
       if(octree)
       {
           std::cout<<"Map received..."<<endl;
            for(OcTree::tree_iterator it = octree->begin_tree(),end = octree->end_tree(); it!= end; ++it)
            {
                if(octree->isNodeOccupied(*it))
                {
                    std::cout<<" x = "<< it.getX()<<endl;
                    std::cout<<" y = "<< it.getY()<<endl;
                    std::cout<<" z = "<< it.getZ()<<endl;
                    std::cout<<"---------------------------------------"<<endl;
                    std::cout<<"Occupancy probability of node: "<<it->getOccupancy()<<endl;
                    occupied++;
                    
                    
                }
                else
                {
                    free++;
                    
                }

                
            }
            std::cout<<"Number of occupied cells = "<<occupied<<endl;
            std::cout<<"Number of Free cells: "<<free<<endl;
       } 

    }

    delete tree;
}

int main (int argc, char **argv)
{

    ros::init(argc,argv, "sub2");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/octomap_binary",1000,ChatterCallback);
    ros::spin();
    return 0;
}