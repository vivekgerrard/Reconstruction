#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>

ros::Publisher pub;
void cb(const sensor_msgs::PointCloud2ConstPtr& ros_cloud)
{
	pcl::PointCloud<pcl::PointXYZ> input;
	pcl::PointCloud<pcl::PointXYZ> filtered;
	pcl::fromROSMsg(*ros_cloud, input);

	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> stat;
  	stat.setInputCloud(input.makeShared());
  	stat.setMeanK (10);
  	stat.setStddevMulThresh(0.2);
	stat.filter (filtered);
	sensor_msgs::PointCloud2 output;
 	pcl::toROSMsg(filtered, output);
	
	pub.publish(output);
}

int main (int argc, char** argv)
{
	ros::init (argc, argv, "stat_filter");
	ros::NodeHandle n;

	ros::Subscriber s = n.subscribe ("output", 1, cb);
	pub = n.advertise<sensor_msgs::PointCloud2> ("outlier_removed", 10);
	ros::spin();
}
