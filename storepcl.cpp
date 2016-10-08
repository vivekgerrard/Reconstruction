#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "std_msgs/String.h"
#include <pcl/io/pcd_io.h>

#include <iostream>

void cb(const sensor_msgs::PointCloud2ConstPtr& ros_cloud)
{
	pcl::PointCloud<pcl::PointXYZ> cloud_in;
	
	pcl::fromROSMsg (*ros_cloud, cloud_in);

	std_msgs::String s;
	std::cin >> s.data;
        pcl::io::savePCDFileASCII (s.data, cloud_in);  

}

int main (int argc, char** argv)
{
	ros::init (argc, argv, "storepcl");
	ros::NodeHandle n;
	ros::Subscriber s = n.subscribe ("outlier_removed", 1, cb);
	ros::spin();
}
