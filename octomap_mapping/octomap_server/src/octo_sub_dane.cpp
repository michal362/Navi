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


int main (int argc, char **argv)
{
    string filename = "mapafile.ot";
    int occupied = 0;
    int free = 0;
    AbstractOcTree* tree = AbstractOcTree::read(filename);
    if(tree)
    {
        OcTree* ot = dynamic_cast<OcTree*>(tree);
        if(ot)
        {
            std::cout<<"Map received..."<<endl;
            for(OcTree::tree_iterator it = ot->begin_tree(),end = ot->end_tree(); it!= end; ++it)
            {
                if(ot->isNodeOccupied(*it))
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
            std::cout<<"Number of Nodes in Tree "<<ot->size()<<endl;
            std::cout<<"pointer of node (cordinates: 2.025,0.775,0.025): "<<ot->search(2.025,0.775,0.025)<<endl;
            
        }
    }
    delete tree;
    return 0;
}
