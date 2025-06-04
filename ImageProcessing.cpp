#include "ImageProcessing.h"

// Init constructor
ImageProcessing::ImageProcessing(int height, int width) {

	this->width = width;
	this->height = height;

	this->build_bar_values();
}

// Values bar of HSV mask
void ImageProcessing::build_bar_values() {

	// Window defination
	namedWindow("Values Bar");
	resizeWindow("Values Bar", Size(500, 500));
	 	
	// HSV Values Bar
	createTrackbar("Hue min", "Values Bar", &this->hmin, 255);
	createTrackbar("Hue max", "Values Bar", &this->hmax, 255);
	createTrackbar("Sat min", "Values Bar", &this->smin, 255);
	createTrackbar("Sat max", "Values Bar", &this->smax, 255);
	createTrackbar("Val min", "Values Bar", &this->vmin, 255);
	createTrackbar("Val max", "Values Bar", &this->vmax, 255);

	// Threshold Values bar
	createTrackbar("Thresh val", "Values Bar", &this->threshold_value, 255);
	createTrackbar("Binary val", "Values Bar", &this->max_binary_value, 255);

	// Warp Values Bar
	createTrackbar("Top Width ", "Values Bar", &this->top_width , 500);
	createTrackbar("Top Height", "Values Bar", &this->top_height, 500);
	createTrackbar("Bot Width ", "Values Bar", &this->bot_width , 500);
	createTrackbar("Bot Height", "Values Bar", &this->bot_height, 500);
}

// Initialize image
Mat ImageProcessing::initialize_image(int scale) {
	return Mat::zeros(Size(int(width / scale), int(height / scale)), CV_8UC3);
}

// Preprocessing frame by resize the shape
Mat ImageProcessing::frame_processing(Mat &frame) {

	resize(frame, frame, Size(this->width, this->height));

	return frame;
}

// Copy image
Mat ImageProcessing::copy_image(Mat &image, int scale) {

	Mat copy_frame;

	image.copyTo(copy_frame);
	
	if (1 < scale) {
		resize(copy_frame, copy_frame, Size(int(this->width / scale), int(this->height / scale)));
	}

	return copy_frame;
}

// Create threshold mask
Mat ImageProcessing::threshold_mask(Mat& image) {

	Mat img;

	cvtColor(image, img, COLOR_BGR2GRAY);

	threshold(img, img, this->threshold_value, this->max_binary_value, THRESH_BINARY);

	return img;
}

// Create HSV mask
Mat ImageProcessing::hsv_mask(Mat &image) {

	Mat imgHSV, imgMask;

	Scalar lower(this->hmin, this->smin, this->vmin);
	Scalar upper(this->hmax, this->smax, this->vmax);

	cvtColor(image, imgHSV, COLOR_BGR2HSV);

	inRange(imgHSV, lower, upper, imgMask);

	return imgMask;
}

// Create Canny Edge mask
Mat ImageProcessing::edge_mask(Mat &image) {

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

	Mat imgGray, imgBlur, imgCanny, imgDilate;

	cvtColor(image, imgGray, COLOR_BGR2GRAY);

	GaussianBlur(imgGray, imgBlur, Size(7, 7), 5, 0);

	Canny(imgBlur, imgCanny, 25, 75);

	dilate(imgCanny, imgDilate, kernel);

	return imgDilate;
}

// Create Warp image
Mat ImageProcessing::warp_image(Mat &img, Mat &src_img, int scale) {

	Mat imgWarp;

	int w = int(this->width / scale);
	int h = int(this->height / scale);

	int top_w = int(top_width / scale);
	int top_h = int(top_height / scale);
	int bot_w = int(bot_width / scale);
	int bot_h = int(bot_height / scale);

	// Float point of source and destiny images
	Point2f dst[4] = { {0.0f,0.0f}, {(float)w, 0.0f}, {0.0f, (float)h}, {(float)w, (float)h} };
	Point2f src[4] = { {(float)top_w, (float)top_h}, {(float)(w - (float)top_w), (float)top_h}, 
					   {(float)bot_w, (float)bot_h}, {(float)(w - bot_w), (float)bot_h} };

	Point2f org[4] = { {(float)top_width, (float)top_height}, {(float)(width - (float)top_width), (float)top_height},
					   {(float)bot_width, (float)bot_height}, {(float)(width - bot_width), (float)bot_height} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(src_img, imgWarp, matrix, Point(w, h));

	// Drawing prespective points on the original image
	for (int i = 0; i < 4; i++) {
		circle(img, org[i], 10, Scalar(0, 0, 255), FILLED);
	}
	return imgWarp;
}

// Function find curve direction
int ImageProcessing::get_curve_direction(Mat &image, Mat &warp_mask_scaled, int scale) {

	// Calculate histogram of only 1/4 image
	int mid_point = this->get_histogram(warp_mask_scaled, 0.5, warp_mask_scaled.rows - int(warp_mask_scaled.rows / 4), scale);

	// Calculate histogram of image
	int average_point = this->get_histogram(warp_mask_scaled, 0.5, 1, scale);

	int curve_direction = mid_point - average_point;

	// Drawing prespective points on the original image
	circle(image, Point2f(float(mid_point * scale), float(image.rows)), 10, Scalar(0, 255, 0), FILLED);
	circle(image, Point2f(float(average_point * scale), float(image.rows)), 10, Scalar(0, 255, 0), FILLED);

	return curve_direction;
}

// Function find base point on histogram image
int ImageProcessing::get_histogram(Mat &mask, float percent, int region, int scale) {
	
	int max = 0, min = 0 ,val = 0;

	int* histogram = new int[int(this->width / scale)];

	// Sum col values and find the max value
	for (int i = 0; i < mask.cols; i++) {

		histogram[i] = 0;	

		for (int j = mask.rows; region <= j; j--) {

			val = mask.at<uchar>(Point(i, j-1));

			histogram[i] = histogram[i] + val;

			// Find the max value of white pixel
			if (max < val) { max = val; }
		}
	}

	// Find the avarage point of the histogram
	int average_point = this->get_curve_point(histogram, percent, max, scale);

	delete []histogram;
	histogram = NULL;

	return average_point;
}

// Function return the base point from image histogram array
int ImageProcessing::get_curve_point(int histogram[], float percent, int max, int scale) {

	int sum_index = 0;
	int counter_index = 0;
	int average_point = 0;

	// Define min value percent to ignore noises
	int min = int(percent * max);

	for (int i = 0; i < int(this->width/scale); i++) {
		if (min < histogram[i]) {
			sum_index = sum_index + i;
			counter_index = counter_index + 1;
		}
	}

	// Find the avarage point of the histogram
	//average_point = int(sum_index / counter_index) * scale;
	average_point = int(sum_index / counter_index);

	return average_point;
}

// Function draw the lane on the screen 
Mat ImageProcessing::draw_lane(Mat& image, Mat &mask_image, int scale) {

	Vec3b color = {0, 255, 0};

	Mat mask_copy, green_img;

	mask_image.copyTo(mask_copy);

	// Initialize img in original frame size
	green_img = this->initialize_image(1);

	// Define the region in image to be green (in this case 1/3 of the image)
	int region = mask_copy.rows - int(mask_copy.rows / 3);

	for (int i = 0; i < mask_copy.cols; i++) {
		for (int j = 0; j < mask_copy.rows; j++) {

			// Set 2/3 pixels from image black
			if (j < region) {
				mask_copy.at<uchar>(Point(i, j)) = 0;
			}
			// Set 1/3 pixels from image green
			else {
				green_img.at<Vec3b>(Point(i, j)) = color;
			}
		}
	}

	// Convert mask_copy from 1 channel to 3 channels;
	cvtColor(mask_copy, mask_copy, COLOR_GRAY2BGR);

	// Create mask in green color
	bitwise_and(mask_copy, green_img, green_img);

	// Connect two frames to one with green curve lane
	addWeighted(image, 1, green_img, 1, 0, image);

	return image;
}
