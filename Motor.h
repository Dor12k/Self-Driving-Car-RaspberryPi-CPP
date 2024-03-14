
# pragma once
# include <thread>
# include <chrono>
//# include <windows.h>
# include <iostream>
# include <wiringPi.h>

using namespace std;

class Motor{

	public:
		Motor(int enableA, int inA1, int inA2, int inB1, int inB2, int enableB);

		int driving_manager(float speed, float turn);

		int get_left_speed(float speed, float turn);
		int get_right_speed(float speed, float turn);

		int move_reverse_right(int left_speed, int right_speed, int distance);
		int move_reverse_left(int left_speed, int right_speed, int distance);
		int move_backward(int left_speed, int right_speed, int distance);
		int move_forward(int left_speed, int right_speed, int distance);
		int move_right(int left_speed, int right_speed, int distance);
		int move_left(int left_speed, int right_speed, int distance);
		int stand();

		int motor_reverse_right(int left_speed, int right_speed, int distance);
		int motor_reverse_left(int left_speed, int right_speed, int distance);
		int motor_backward(int left_speed, int right_speed, int distance);
		int motor_forward(int left_speed, int right_speed, int distance);
		int motor_right(int left_speed, int right_speed, int distance);
		int motor_left(int left_speed, int right_speed, int distance);
		void motor_stand();

private:

	int interval = 1;

	int max_speed = 0;

	int SDK_SPEED = 50;

	int movement_speed = 0;

	// Store the motor SDK speed
	int motor_sdk_angular_speed = 50;
	int motor_sdk_movement_speed = 50;

	// Represent speed
	int motor_angular_speed = int(360 / 10);    // Angular speed degrees / s(50d / s)
	int motor_movement_speed = int(390 / 10);  // Forward speed in cm / s(50cm / s)

	int enableA, inA1, inA2, inB1, inB2, enableB;

	string last_cmd = "";
};
