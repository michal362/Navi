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
    string btFilename = "my_octomap.bt";
    ros::init(argc,argv,"octo_sub_dane");
    ros::NodeHandle k;
    ros::Publisher chatter_pub = k.advertise<std_msgs::String>("chatter",1000);

    ros::Rate loop_rate(10);

    OcTree* tree = new OcTree(btFilename);
    int count = 0;

    while (ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;

        ss<<"hello world"<< count;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());

        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;

        for(OcTree::leaf_iterator it = tree->begin_leafs(),end = tree->end_leafs(); it!= end; ++it)
        {
            cout<<"Node center: "<<it.getCoordinate();
            cout<<" Value: "<<it->getValue()<<"\n";
        }


    }

    return 0;
}
