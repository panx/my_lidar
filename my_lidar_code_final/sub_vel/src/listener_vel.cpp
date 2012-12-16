#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "math.h"
#include <cereal_port/CerealPort.h>
#include "std_msgs/Float64.h"
#define PI 3.1416
double rec_vel_x=0.0;
double rec_vel_y=0.0;
double base_vel=0.0;
double base_angle=0.0;
int base_angle_int=0;
cereal::CerealPort device;
char ang_send[1];
int flag = 0;
int old_base_angle_int = 0;
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void cmd_vel_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
  	double temp_val;

	rec_vel_x = msg->linear.x;
	rec_vel_y = msg->linear.y;
        base_angle = msg->angular.z;
	if(rec_vel_x == 0 && rec_vel_y ==0)
	{
		ang_send[0]='x';
		base_vel = 0.0;
		base_angle = 0;
	}
        else{
		if(base_angle>0){
                    ang_send[0] ='s';
		    base_vel = 0.3;
                }
                else if(base_angle<0){
		    ang_send[0]='a';
                    base_vel=0.3;
                }
                else if(base_angle==0){
                    ang_send[0]='z';
                    base_vel=0.22;
                }
 
        }
  device.write(ang_send,1);  

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener_vel");

  ros::NodeHandle n;
   // char reply[REPLY_SIZE];
  try{ device.open("/dev/ttyACM1", 9600); } 
    catch(cereal::Exception& e)
    {
        ROS_FATAL("Failed to open the serial port!!!");
        ROS_BREAK();
    }
    ROS_INFO("The serial port is opened.");
  ros::Subscriber sub = n.subscribe("/cmd_vel", 1000, cmd_vel_Callback);
  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("act_vel", 1000);
  ros::Publisher ang_pub = n.advertise<std_msgs::Float64>("act_th", 1000);

 // ros::Rate loop_rate(10);
 ros::Rate loop_rate(18);	
 while(ros::ok())
    {

  	ros::spinOnce();

	std_msgs::Float64 th;
	geometry_msgs::Twist vel;
	
	vel.linear.x = rec_vel_x; 
	vel.linear.y = rec_vel_y;
	
	th.data = base_angle;

	vel_pub.publish(vel);
	ang_pub.publish(th);
	loop_rate.sleep();
  }


  return 0;
}
