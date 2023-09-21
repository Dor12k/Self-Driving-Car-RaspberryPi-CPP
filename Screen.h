
# pragma once
# include <iostream>
# include <opencv2/highgui.hpp>

# include <opencv2/imgproc.hpp>
# include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

class Screen
{
public:

	void display_frame(Mat image);
	void design_frame(Mat &image, int fps);

};

