
# pragma once
# include <iostream>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

class RemoteControl
{
public:
	int get_user_input();
	int system_functional(int key);

	float get_turn(int key);
	float get_speed(int key);

	bool system_on(int key, bool system_on);
	bool self_driving(int key, bool self_driving);

};

