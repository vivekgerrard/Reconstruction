#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

ros::Publisher pub;
void cb(const sensor_msgs::PointCloud2ConstPtr& ros_cloud)
{
	pcl::PointCloud<pcl::PointXYZ> input;
	pcl::PointCloud<pcl::PointXYZ> downsampled;
	pcl::fromROSMsg(*ros_cloud, input);

	pcl::VoxelGrid<pcl::PointXYZ> voxel;
  	voxel.setInputCloud(input.makeShared());
  	voxel.setLeafSize(0.01f, 0.01f, 0.01f);
	voxel.filter (downsampled);
	sensor_msgs::PointCloud2 output;
 	pcl::toROSMsg(downsampled, output);
	
	pub.publish(output);
}

int main (int argc, char** argv)
{
	ros::init (argc, argv, "downsampling");
	ros::NodeHandle n;

	ros::Subscriber s = n.subscribe ("outlier_removed", 1, cb);
	pub = n.advertise<sensor_msgs::PointCloud2> ("downsampled", 10);
	ros::spin();
}
