// Include libraries/header files

#include <ros/ros.h>
#include <ball_chaser/DriveToTarget.h>
#include <sensor_msgs/Image.h>

using namespace std;

// Global client
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specific direction
void drive_robot(float lin_x, float ang_z)
{
   ROS_INFO_STREAM("Driving the robot...");

   ball_chaser::DriveToTarget srv;
   srv.request.linear_x = lin_x;
   srv.request.angular_z = ang_z;

   if(!client.call(srv))
   {
      ROS_ERROR("Failed to service drive_robot");
   }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
   int num_pixels  = img.height*img.step;
   int high_value = 255;
   int left_ub = img.width/3;
   int center_ub = 2*left_ub;
   int right_ub = 3*left_ub;
   int loop_count = 0;
   int column = 0;
   float lin_x = 0.0;
   float ang_z = 0.0;

   for(int i = 0; i < (num_pixels); i += 3)
   {
      if((img.data[i] == high_value) && (img.data[i+1] == high_value) && (img.data[i+2] == high_value))
      {
         ROS_INFO("Detected white pixel!");
         
         // Find location
   
         ROS_INFO("Looking for location...");
         
         column = int(i/3) % img.width;

         if((column >= 0) && (column < left_ub))
         {
            // Rotate to the left
            lin_x = 0.0;
            ang_z = 0.1;
         }

         // Center Frame
         else if((column >= left_ub) && (column < center_ub))
         {
            // Move forward
            lin_x = 0.1;
            ang_z = 0.0;
         }

         // Right Frame
         else
         {
            // Rotate to the right
            lin_x = 0.0;
            ang_z = -0.1;
         }

         break;

      }
   }

   // Exectue controls
   drive_robot(lin_x,ang_z);

}

int main(int argc, char** argv) // arguments?
{
   // Initialize the process_image node and create a handle to it
   ros::init(argc,argv,"process_image");
   ros::NodeHandle n;

   // Define a client service capable of requesting services from command_robot
   client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

   // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
   ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

   // Handle ROS communication events
   ros::spin();

   return 0;
}
