#pragma once

# include <iostream>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

class ImageProcessing {

	public:
		ImageProcessing(int heigh, int width);
	
		void build_bar_values();

		Mat initialize_image(int scale);
		Mat frame_processing(Mat &frame);
		Mat copy_image(Mat &image, int scale);

		Mat hsv_mask(Mat &image);
		Mat edge_mask(Mat &image);
		Mat threshold_mask(Mat& image);
		Mat warp_image(Mat &image, Mat &img, int scale);

		Mat draw_lane(Mat &image, Mat &mask, int scale);
		Mat draw_lane2(Mat& image, Mat mask, int scale);

		int get_curve_point(int histogram[], float percent, int max, int scale);
		int get_curve_direction(Mat &image, Mat &mask, int scale);

		int get_histogram(Mat &img, float percent, int region, int scale);

	private:

		int height = 350, width = 500;

		// HSV mask initialized values
		int hmin = 0, smin = 5, vmin = 159;
		int hmax = 31, smax = 69, vmax = 255;

		// Warp mask initialized values
		int top_width = 55, top_height = 210;
		int bot_width = 10, bot_height = 337;

		// Threshold mask initialized values
		int threshold_value = 195, max_binary_value = 255;

};

