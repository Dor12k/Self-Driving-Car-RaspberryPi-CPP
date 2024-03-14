
# pragma once
# include <iostream>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2/imgcodecs.hpp>
# include <opencv2/tracking.hpp>

#include <cmath>
#include <fstream>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/examples/label_image/get_top_n.h"
#include "tensorflow/lite/model.h"

using namespace cv;
using namespace std;

class AI{

	public:

		float get_traffic_sign(Mat& image, Mat& copy_image, Mat& edge_mask, int scale);
		float traffic_sign_tracking(Mat& image, Mat& copy_image, Mat& edge_mask, int scale);
		float traffic_sign_detection(Mat& image, Mat& edge_mask_scaled);

		string get_prediction(Mat& image);
		string get_prediction2(Mat& img);


		Rect getDetection(Mat &image, Mat edge_mask);
		void draw_rectangle(Mat &image, Rect boundingRect, string label);

		float aim_car_direction(int curve_direction);
		float get_average_direction(int curve_direction);

		bool getFileContent(string fileName);

		bool traffic_sign_translation(string label);

		void init_detection();

		Mat preprocess_image(Mat& image, int model_width, int model_height);

	private:

		int speed = 50;

		int curve_counter = 0;

		float max_speed = 0.5f;

		string traffic_sign = "";

		bool onTracking = false;
		bool trafficSignDetected = false;

		int curve_list[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		Ptr<Tracker> tracker = TrackerKCF::create();
		//Ptr<Tracker> tracker = Tracker::create("KCF");

		std::vector<std::string> Labels;
		//std::unique_ptr<tflite::Interpreter> interpreter;
};