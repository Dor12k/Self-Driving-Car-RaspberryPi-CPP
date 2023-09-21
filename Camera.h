
# include <opencv2/imgcodecs.hpp>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <iostream>
# include <chrono>
#include <ctime>
# pragma once

using namespace cv;
using namespace std;

using namespace std::chrono;

class Camera
{
public:
	Camera();
	Camera(int id);

	Mat read_frames();

	int get_fps();
	void set_fps(int fps);

private:

	const int HEIGHT = 350, WIDTH = 500;

	string path = "Resource/Road 24.MOV";

	VideoCapture camera;

	Mat img;

	int fps = 0;

	int FPS = 0;

	clock_t start = clock();
	

};

