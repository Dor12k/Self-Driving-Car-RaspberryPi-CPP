
# pragma once
# include <iostream>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2/imgcodecs.hpp>


using namespace cv;
using namespace std;

class AI {
	public:

		float get_traffic_sign(Mat& image, Mat& copy_image, Mat& edge_mask, int scale);
		float traffic_sign_tracking(Mat& image, Mat& copy_image, Mat& edge_mask, int scale);
		float traffic_sign_detection(Mat& image, Mat& edge_mask_scaled);

		string get_prediction(Mat& image);
		Rect getDetection(Mat &image, Mat edge_mask);
		void draw_rectangle(Mat &image, Rect boundingRect, string label);

		float aim_car_direction(int curve_direction);
		float get_average_direction(int curve_direction);

		bool traffic_sign_translation(string label);

	private:

		int speed = 50;

		int curve_counter = 0;

		float max_speed = 0.5f;

		string traffic_sign = "";

		bool trafficSignDetected = false;

		int curve_list[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		//Ptr<Tracker> tracker = Tracker::create("KCF");
};

