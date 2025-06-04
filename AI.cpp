
#include "AI.h"

// Function manage the detection and tracking options
float AI::get_traffic_sign(Mat& image, Mat& copy_image, Mat& edge_mask, int scale) {


	//cout << this->max_speed << endl;
	if (trafficSignDetected = true) {
		this->max_speed = this->traffic_sign_detection(image, edge_mask);
		//this->max_speed = this->traffic_sign_tracking(image, copy_image, edge_mask, scale);
	}
	else {
		this->max_speed = this->traffic_sign_detection(image, edge_mask);
	}
	return this->max_speed;
}

// Function make camera tracking after the traffic sign
float AI::traffic_sign_tracking(Mat& image, Mat& copy_image, Mat& edge_mask, int scale) {

	string label;
	Rect boundRect;


	if (true) {

		//// If tracking update success then drawing rectangle
		//if (this->tracker->update(copy_image, boundRect)) {

		//	int x = boundRect.x * scale;
		//	int y = boundRect.y * scale;
		//	int w = boundRect.width * scale;
		//	int h = boundRect.height * scale;

		//	//int x = boundRect.x;
		//	//int y = boundRect.y;
		//	//int w = boundRect.width;
		//	//int h = boundRect.height;
		//	x = x + int(image.cols - (image.cols / 4));
		//	y = y + int(image.rows / 4);
		//	//x = x + int(image.cols - (image.cols / 2));
		//	//y = y + int(image.rows / 4);

		//	Rect tf_boundingBox(x, y, w, h);

		//	this->draw_rectangle(image, tf_boundingBox, this->traffic_sign);
		//}
		//else {
		//	this->onTracking = false;
		//	this->trafficSignDetected = false;
		//}
	}
	this->draw_rectangle(image, boundRect, label);

	return this->max_speed;
}

// Function detect traffic sign
float AI::traffic_sign_detection(Mat& image, Mat& edge_mask) {

	Rect roi(int(edge_mask.cols / 2), int(0), int(edge_mask.cols / 2), int(edge_mask.rows / 2));

	Mat mask_crop = edge_mask(roi);
	Mat img_crop = image(roi);

	Rect boundingBox = this->getDetection(img_crop, mask_crop);

	// If traffic sign detected then start tracking
	if (!boundingBox.empty()) {

		bool onTracking = true;
		this->trafficSignDetected = true;

		// Here need to be tracker init and speed
		//Ptr<Tracker> tracker = TrackerKCF::create();
		//this->tracker->init(image, boundingBox);
	}

	return this->max_speed;
}

// Function detect traffic sign
Rect AI::getDetection(Mat& image, Mat edge_mask_crop) {

	Mat fragment;

	string label = "";

	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	findContours(edge_mask_crop, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// Scan all the contours and search for traffic sign
	for (int i = 0; i < contours.size(); i++) {

		int area = (int)contourArea(contours[i]);
		//cout << area << endl;

		if (100 < area and area < 500) {

			// Cut the ROI from image
			fragment = image(boundingRect(contours[i]));

			//label = this->get_prediction(fragment);

			// Using for test
			string path = "Resource/TF_stop1.png";
			cv::Mat image_tf = cv::imread(path);
			label = this->get_prediction(image_tf);
			bool detected_traffic_sign = this->traffic_sign_translation(label);

			if (detected_traffic_sign == true) {

				cout << "Area2: " << area << endl;
				this->traffic_sign = label;
				return boundingRect(contours[i]);
			}
		}
	}
	Rect failed(0, 0, 0, 0);
	return failed;
	//return Rect(NULL, NULL, NULL, NULL);
}

// Function translate traffic sign to a command
bool AI::traffic_sign_translation(string label) {

	bool relevant_label = true;

	// Remove chae '\r' from the string label
	label.erase(std::remove(label.begin(), label.end(), '\r'), label.end());

	if (label == "Stop") {

		this->max_speed = 0.0f;
		this->traffic_sign = label;
	}
	else {
		if (label == "No entry") {

			this->max_speed = 0.0f;
			this->traffic_sign = label;
		}
		else {
			if (label == "Pedestrians") {

				this->max_speed = 0.0f;
				this->traffic_sign = label;
			}
			else {
				if (label == "Speed limit (30km/h)") {

					this->max_speed = 0.3;
					this->traffic_sign = label;
				}
				else {
					if (label == "Speed limit (50km/h)") {
						this->max_speed = 0.5;
						this->traffic_sign = label;
					}
					else {
						// Label not in the relevant bank
						relevant_label = false;
					}
				}
			}
		}
	}
	return relevant_label;
}

// Function doing the model prediction
string AI::get_prediction(Mat& image) {

	string label = "";

	std::cout << "     " << std::endl;
	std::cout << "image size: " << image.size << std::endl;

	// Reshape image to tensor
	resize(image, image, Size(48, 48), 0, 0, INTER_LINEAR);

	int size_2[4] = {1, image.rows, image.cols, image.channels()};

	cv::Mat tensor_img(4, size_2, image.type(), image.data);

	std::cout << "tensor_img size: " << tensor_img.size << std::endl;

	std::cout << "tensor_img dims = " << tensor_img.dims << " tensor_img size = [";
	for (int i = 0; i < tensor_img.dims; ++i) {
		if (i) std::cout << " X ";
		std::cout << tensor_img.size[i];
	}
	std::cout << "] tensor_img channels = " << tensor_img.channels() << std::endl;
	cout << " " << endl;

	// HERE NEED TO BE A MODEL

	//Check for rpi4
	cv::Mat grey_img;
	cv::Mat equalize_img;

	imshow("image", image);

	// Create greyscale img
	image.copyTo(grey_img);
	cv::cvtColor(grey_img, grey_img, COLOR_BGR2GRAY);
	imshow("grey_img", grey_img);

	// Create equalize img
	equalizeHist(grey_img, equalize_img);
	imshow("equalize_img", equalize_img);

	// Normalize img
	grey_img.convertTo(grey_img, CV_32FC1, 1 / 255.0);

	cout << " " << endl;
	cout << "grey_img Type: " << grey_img.type() << endl;
	cout << "grey_img Channel: " << grey_img.channels() << endl;

	// Create tensorflow grey img
	int size_3[4] = {1, grey_img.rows, grey_img.cols, grey_img.channels() };
	cv::Mat tensor_grey_img(4, size_3, grey_img.type(), grey_img.data);

	std::cout << "tensor_grey_img.dims = " << tensor_grey_img.dims << " tensor_grey_img.size = [";
	for (int i = 0; i < tensor_grey_img.dims; ++i) {
		if (i) std::cout << " X ";
		std::cout << tensor_grey_img.size[i];
	}
	std::cout << "] tensor_grey_img.channels = " << tensor_grey_img.channels() << std::endl;

	return label;
}

// Function drawing rectangle on the image
void AI::draw_rectangle(Mat& image, Rect boundRect, string label) {

	rectangle(image, boundRect.tl(), boundRect.br(), Scalar(0, 255, 0), 3);
	putText(image, label, { boundRect.x, boundRect.y - 5 }, FONT_HERSHEY_COMPLEX, 0.7, Scalar(175, 50, 175), 2);
}

// Function find the car direction acoording to the curve direction
float AI::aim_car_direction(int curve_direction) {

	float sensitivity = 1.3f;

	float max_speed = float(float(this->speed) / 100.f);

	float average_curve = this->get_average_direction(curve_direction);

	// Limit the speed
	if (max_speed < average_curve) {
		average_curve = max_speed;
	}
	if (average_curve < -max_speed) {
		average_curve = -max_speed;
	}

	if (0 < average_curve) {
		sensitivity = 1.7f;
		if (average_curve < 0.05) {
			average_curve = 0;
		}
	}
	else {
		if (-0.08 < average_curve) {
			average_curve = 0;
		}
	}

	float turn = (average_curve * sensitivity);
	//cout << "Turn " << turn << endl;
	return turn;
}

// Function calculate the average of curve direction
float AI::get_average_direction(int curve_direction) {

	float sum = 0;

	int average_number = 10;

	float average_curve = 0;

	this->curve_list[this->curve_counter] = curve_direction;

	this->curve_counter += 1;

	// Init the counter in case is large the length array
	if (average_number <= this->curve_counter) {
		this->curve_counter = 0;
	}

	// Sum the elements to find average
	for (int i = 0; i < average_number; i++) {
		sum = sum + curve_list[i];
	}

	// Convert average in decimal
	average_curve = (sum / average_number) / 10;

	// Normalize the average value
	if (1 < average_curve) {
		average_curve = 1;
	}
	if (average_curve < -1) {
		average_curve = -1;
	}

	return average_curve;
}
