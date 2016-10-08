#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>

ros::Publisher pub;
void cb(const sensor_msgs::PointCloud2ConstPtr& ros_cloud)
{
	pcl::PCLPointCloud2* p= new pcl::PCLPointCloud2;
	pcl::PCLPointCloud2ConstPtr pPtr(p);
	pcl_conversions::toPCL(*ros_cloud, *p);
	pcl::PCLPointCloud2 p_filtered;
	
	pcl::PassThrough<pcl::PCLPointCloud2> pass;
  	pass.setInputCloud(pPtr);
  	pass.setFilterFieldName ("z");
  	pass.setFilterLimits (1.3, 2.0);
	pass.filter (p_filtered);
	sensor_msgs::PointCloud2 output;
 	pcl_conversions::fromPCL(p_filtered, output);
	
	pub.publish(output);
}

int main (int argc, char** argv)
{
	ros::init (argc, argv, "passthrough");
	ros::NodeHandle n;

	ros::Subscriber s = n.subscribe ("/camera/depth/points", 1, cb);
	pub = n.advertise<sensor_msgs::PointCloud2> ("output", 1);
	ros::spin();
}
