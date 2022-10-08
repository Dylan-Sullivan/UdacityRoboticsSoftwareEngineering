Ball Chaser is a node that can be launched that has the virtual mobile robot use an RGB-D camera to detect a purely white ball within the virtual world by stepping through a 1D representation of the matrix representing the image in terms of pixel mappings (see process_image.cpp).

Then, based on the feed back from process_image, another program (drive_bot.cpp) could be used to control the robot to drive towards the white ball.
