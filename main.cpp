
//This version using to preview the project in github

# include "AI.h"
# include "Motor.h"
# include "Screen.h"
# include "Camera.h"
# include "RemoteControl.h" 
# include "ImageProcessing.h"

# include <thread>
# include <iostream>

using namespace cv;
using namespace std;
using namespace std::chrono;

int main() {

	int key;
	int FPS = 0;
	int scale = 4;

	float speed = 0.5f;
	float turn = 0.f;

	int curve_direction;

	bool system_on = true;
	bool self_driving = false;

	auto timer_start = steady_clock::now();

	AI ai;

	Screen screen;

	Camera camera(2);

	RemoteControl remoteControl;

	Motor motor(2, 3, 4, 17, 27, 22);

	ImageProcessing imgProcessing(350, 500);

	Mat frame, mask_frame, copy_frame_scaled, mask_frame_scaled, warp_frame_scaled, edge_frame_scaled;

	warp_frame_scaled = imgProcessing.initialize_image(scale);

	edge_frame_scaled = imgProcessing.initialize_image(scale);

	while(system_on) {

		frame = camera.read_frames();
		
		frame = imgProcessing.frame_processing(frame);

		//mask_frame = imgProcessing.hsv_mask(frame);
		mask_frame = imgProcessing.threshold_mask(frame);

		copy_frame_scaled = imgProcessing.copy_image(frame, scale);

		mask_frame_scaled = imgProcessing.hsv_mask(copy_frame_scaled);
		//mask_frame_scaled = imgProcessing.threshold_mask(copy_frame_scaled);

		// Get input keyboard from user
		key = remoteControl.get_user_input();

		// Check if system flag is on or off
		system_on = remoteControl.system_on(key, system_on);

		// Check if self-driving mode is on or off
		self_driving = remoteControl.self_driving(key, self_driving);

		// Start Self-Driving mode
		if (self_driving == true) {

			speed = 0.5f;

			// Function returns edge mask of the frame
			edge_frame_scaled = imgProcessing.edge_mask(copy_frame_scaled);

			// Function returns warp image of the curve
			warp_frame_scaled = imgProcessing.warp_image(frame, mask_frame_scaled, scale);

			// Get histogram of image
			curve_direction = imgProcessing.get_curve_direction(frame, warp_frame_scaled, scale);

			// Draw lane curve on the frame
			frame = imgProcessing.draw_lane(frame, mask_frame, scale);

			// Should be traffic sign detection
			speed = ai.get_traffic_sign(frame, copy_frame_scaled, edge_frame_scaled, scale);
			//speed = ai.traffic_sign_detection(copy_frame_scaled, edge_frame_scaled);

			// Get the turn value for driving
			turn = ai.aim_car_direction(curve_direction);

		}
		// User Remote Control mode
		else {
			// Get turn value from user
			turn = remoteControl.get_turn(key);

			// Get speed value from user
			speed = remoteControl.get_speed(key);
		}

		//int x = motor.driving_manager(speed, turn);
		thread left_thread1(&Motor::driving_manager, &motor, speed, turn);
		if (left_thread1.joinable()) {
			left_thread1.detach();
		}

		// Print frames on the screen
		screen.design_frame(frame, camera.get_fps(), speed);
		screen.display_frame(frame);

		//imshow("Frame", frame);
		//imshow("Frame", copy_frame_scaled);
		imshow("Mask", mask_frame);
		imshow("Warp", warp_frame_scaled);
	}

	destroyAllWindows();
	return 1;
}