#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>
ros::Publisher pub;
int i;
pcl::PointCloud<pcl::PointXYZ> cloud_out;
void cb(const sensor_msgs::PointCloud2ConstPtr& ros_cloud)
{
	pcl::PointCloud<pcl::PointXYZ> cloud_in;
	
	pcl::PointCloud<pcl::PointXYZ> cloud_aligned;
	pcl::fromROSMsg (*ros_cloud, cloud_in);

	if (i==0)
	{
		cloud_out = cloud_in;
		i++;
	}
	pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
	icp.setInputSource(cloud_in.makeShared());	
	icp.setInputTarget(cloud_out.makeShared());
	icp.setMaxCorrespondenceDistance(5);
	icp.setMaximumIterations(10);
	icp.setTransformationEpsilon(1e-12);
	icp.setEuclideanFitnessEpsilon(0.1);
	
	icp.align(cloud_aligned);

	sensor_msgs::PointCloud2 output;
	pcl::toROSMsg(cloud_aligned, output);
	pub.publish(output);
	

	cloud_out = cloud_aligned;
}

int main (int argc, char** argv)
{
	ros::init (argc, argv, "matching");
	ros::NodeHandle n;
	i =0;
	ros::Subscriber s = n.subscribe ("output", 1, cb);
	pub = n.advertise<sensor_msgs::PointCloud2> ("output1", 1);
	ros::spin();
}
