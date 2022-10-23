#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
// Included DriveToTarget header file
#include "ball_chaser/DriveToTarget.h"
#include <std_msgs/Float64.h>

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
   // TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
   // This function should publish the requested linear x and angular z velocities to the robot wheel joints
   // After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
   
   ROS_INFO("linear.x = %.4f, angular_z = %.4f", req.linear_x, req.angular_z);

   //ROS_INFO("DriveToTargetRequest received - something:%1.2f", req.);
   
   

   // Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values
   
   // Create a motor_command object of type geometry_msgs::Twist
   geometry_msgs::Twist motor_command;
   // Set wheel velocities, forward [0.5, 0.0]
   motor_command.linear.x = req.linear_x;
   motor_command.angular.z = req.angular_z;
   // Publish angles to drive the robot
   motor_command_publisher.publish(motor_command);

   return true;
}

int main(int argc, char** argv)
{
   // Initialize a ROS node
   ros::init(argc, argv, "drive_bot");

   // Create a ROS NodeHandle object
   ros::NodeHandle n;

   // Inform ROS master that we will e publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
   motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);

   // Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
   ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

   // Handle ROS communication events
   ros::spin();

   return 0;
}
