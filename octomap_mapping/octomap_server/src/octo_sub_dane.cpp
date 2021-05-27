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
            for(OcTree::leaf_iterator it = ot->begin_leafs(),end = ot->end_leafs(); it!= end; ++it)
            {
                if(ot->isNodeOccupied(*it))
                {
                    std::cout<<" x = "<< it.getX()<<endl;
                    std::cout<<" y = "<< it.getY()<<endl;
                    std::cout<<" z = "<< it.getZ()<<endl;
                    std::cout<<"---------------------------------------"<<endl;
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
    return 0;
}
