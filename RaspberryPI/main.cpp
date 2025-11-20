

# include <ctime>
# include <chrono>
# include <iostream>
# include <opencv2/highgui.hpp>
# include <opencv2/imgproc.hpp>
# include <opencv2/imgcodecs.hpp>
# include <opencv2/tracking.hpp>

# include "tensorflow/lite/interpreter.h"
# include "tensorflow/lite/kernels/register.h"
# include "tensorflow/lite/string_util.h"
# include "tensorflow/lite/examples/label_image/get_top_n.h"
# include "tensorflow/lite/model.h"
# include <wiringPi.h>
# include <lccv.hpp>

using namespace cv;
using namespace std;


Mat frame, matrix, framePers, frameThresh, frameGray, frameEdge, frameFinal, frameFinalDuplicate, frameFinalDuplicate2, ROILane, ROILaneEnd;

// 1=vid, 2=pi
int camera = 2;

//VideoCapture vid_camera;

lccv::PiCamera pi_camera;

vector<int> histrogramCurve;
vector<int> histrogramLane;
vector<int> histrogramLaneEnd;

int speed = 50;

int const WIDTH = 360, HEIGHT = 240;

int laneEnd, laneEndLimit = 3000;
int leftLanePos, rightLanePos, centerLanePos, frameCenter = (int(WIDTH / 2) - 1), laneCurve;

stringstream ss;

Point2f p1;
Point2f p2;


bool selfDrivingMode = false;

auto start = std::chrono::system_clock::now();

int thresholdV = 100; // 140

// Warp mask initialized values
int top_width = 50;
int top_height = 160;
int bot_width = 1;
int bot_height = 210;

// Prespective ROI coordinates
Point2f left_top = Point2f(top_width, top_height);
Point2f right_top = Point2f(WIDTH - top_width, top_height);
Point2f left_bot = Point2f(bot_width, bot_height);
Point2f right_bot = Point2f(WIDTH - bot_width, bot_height);

Point2f source2[] = {left_top, right_top, left_bot, right_bot};

// Prespective ROI coordinates
Point2f source[] = {Point2f(int(WIDTH / 10) + 10, HEIGHT - int(HEIGHT / 3) + 10), Point2f(WIDTH - int(WIDTH / 10) - 10,HEIGHT - int(HEIGHT / 3) + 10),
					Point2f(0, HEIGHT - int(HEIGHT / 10) - 6), Point2f(WIDTH, HEIGHT - int(HEIGHT / 10) - 6) };

// Warp ROI coordinates
Point2f destination[] = {Point2f(int(WIDTH / 4.5), 0), Point2f(WIDTH - int(WIDTH / 4.5), 0),
						 Point2f(int(WIDTH / 4.5), HEIGHT), Point2f(WIDTH - int(WIDTH / 4.5), HEIGHT) };


// Machine Learning variables
int distance_counter = 0;
string lightColor = "Traffic Light";

Rect trackingWindow; // Region to track
Ptr<Tracker> tracker = TrackerKCF::create(); // Using KCF tracker

// Stop Sign variables
int distance_stop;
bool stopSignDetected = false;
vector<Rect> Stop;
CascadeClassifier Stop_Cascade;
Mat frame_stop, ROI_stop, gray_frame_stop;


// Obstacle variables
int distance_obstacle;
bool obstacleDetected = false;
vector<Rect> Obstacle;
Mat frame_obstacle, ROI_obstacle, gray_frame_obstacle;
CascadeClassifier Obstacle_Cascade;



// Traffic Light Variables
int distance_trafficLight;
bool trafficLightDetected = false;
vector<Rect> trafficLight;
Mat frame_trafficLight, ROI_trafficLight, gray_frame_trafficLight;
CascadeClassifier TrafficLight_Cascade;

bool detectMode = true;

// Function create the values bar
void buildBar() {

	// Window defination
	namedWindow("Values Bar");
	moveWindow("Values Bar", 800, 500);
	resizeWindow("Values Bar", Size(500, 200));

	// HSV Values Bar
	createTrackbar("Threshold", "Values Bar", &thresholdV, 255);

	// Warp Values Bar
	createTrackbar("Top Width", "Values Bar", &top_width, WIDTH);
	createTrackbar("Top Height", "Values Bar", &top_height, HEIGHT);
	createTrackbar("Bot Width", "Values Bar", &bot_width, WIDTH);
	createTrackbar("Bot Height", "Values Bar", &bot_height, HEIGHT);
}

// Function Draw line on the ROI frame according to the bar values
void barPerspective() {

	// next lines is for track bar
	top_width = getTrackbarPos("Top Width", "Values Bar");
	top_height = getTrackbarPos("Top Height", "Values Bar");
	bot_width = getTrackbarPos("Bot Width", "Values Bar");
	bot_height = getTrackbarPos("Bot Height", "Values Bar");

	Point2f left_top = Point2f(top_width, top_height);
	Point2f right_top = Point2f(WIDTH-top_width, top_height);
	Point2f left_bot = Point2f(bot_width, bot_height);
	Point2f right_bot = Point2f(WIDTH - bot_width, bot_height);

	Point2f source1[] = {left_top, right_top, left_bot, right_bot};

	line(frame, source1[0], source1[1], Scalar(0, 0, 255), 2);
	line(frame, source1[1], source1[3], Scalar(0, 0, 255), 2);
	line(frame, source1[3], source1[2], Scalar(0, 0, 255), 2);
	line(frame, source1[2], source1[0], Scalar(0, 0, 255), 2);

	matrix = getPerspectiveTransform(source1, destination);
	warpPerspective(frame, framePers, matrix, Size(WIDTH, HEIGHT));
}

// Function set camera defination
void setCamera(int cameraID) {

	if (cameraID == 1) {

		string path = "Resource/Road 24.MOV";
//		//string path = "Resource/video5.avi";
//
//		vid_camera.set(CAP_PROP_FRAME_HEIGHT, HEIGHT);
//		vid_camera.set(CAP_PROP_FRAME_WIDTH, WIDTH);
//
//		vid_camera.set(CAP_PROP_SATURATION, 50);
//		vid_camera.set(CAP_PROP_BRIGHTNESS, 50);
//		vid_camera.set(CAP_PROP_CONTRAST, 50);
//		vid_camera.set(CAP_PROP_GAIN, 50);
//		vid_camera.set(CAP_PROP_FPS, 30);
//
//		vid_camera.open(path);
	}
	else {
		if (cameraID == 2) {
			// Using Pi Camera

			pi_camera.options->video_width = WIDTH;
			pi_camera.options->video_height = HEIGHT;

            //need to check the frame rate value
			pi_camera.options->framerate = 30;
			pi_camera.options->verbose = true;
			pi_camera.startVideo();
		}
	}
}

// Function capture frames from camera
void captureFrames() {

	if (camera == 1) {
		//vid_camera.read(frame);

		if (frame.empty()) {
			frame = Mat::zeros(Size(WIDTH, HEIGHT), CV_8UC3);
		}
	}
	else {
		if (camera == 2) {
			pi_camera.getVideoFrame(frame, 1000);

			if (frame.empty()) {
				frame = Mat::zeros(Size(int(WIDTH), int(HEIGHT)), CV_8UC3);
				cout << "Loading.. " << endl;
			}
		}
		else {
			if (frame.empty()) {
				frame = Mat::zeros(Size(int(WIDTH), int(HEIGHT)), CV_8UC3);
			}
		}
	}

	// Need to check the CAP_PROP_FRAME
	resize(frame, frame, Size(WIDTH, HEIGHT));
}

// Function Draw line on the ROI frame
void Perspective() {

	line(frame, source2[0], source2[1], Scalar(0, 0, 255), 2);
	line(frame, source2[1], source2[3], Scalar(0, 0, 255), 2);
	line(frame, source2[3], source2[2], Scalar(0, 0, 255), 2);
	line(frame, source2[2], source2[0], Scalar(0, 0, 255), 2);

	matrix = getPerspectiveTransform(source2, destination);
	warpPerspective(frame, framePers, matrix, Size(WIDTH, HEIGHT));
}

// Function create threshold mask
void threshold() {

	//thresholdV = getTrackbarPos("Threshold", "Values Bar");

	cvtColor(framePers, frameGray, COLOR_RGB2GRAY);
	//GaussianBlur(frameGray, frameGray, Size(15, 15), 0);
	inRange(frameGray, thresholdV, 255, frameThresh);
	Canny(frameGray, frameEdge, 690, 750, 3, false);
	add(frameThresh, frameEdge, frameFinal);
	cvtColor(frameFinal, frameFinal, COLOR_GRAY2RGB); // transformation to 3 channels

	// frameFinalDuplicate used in histrogram function only
	// copy frames by cvt function
	cvtColor(frameFinal, frameFinalDuplicate, COLOR_RGB2BGR);
	cvtColor(frameFinal, frameFinalDuplicate2, COLOR_RGB2BGR);
	cvtColor(frameFinal, frameFinal, COLOR_RGB2BGR);
}

// Function calculate the histrogram of the lane
void histrogram() {

	// frameFinal is a warp mask and not original frame
	histrogramLane.resize(WIDTH);
	histrogramLane.clear();

	histrogramLaneEnd.resize(WIDTH);
	histrogramLaneEnd.clear();

	for (int i = 0; i < frame.size().width; i++) {

        if(0 < i && i < 350){
            ROILane = frameFinalDuplicate(Rect(i, HEIGHT - int(HEIGHT / 3), 1, int(HEIGHT / 3)));
            divide(255, ROILane, ROILane);
            histrogramLane.push_back((int)(sum(ROILane)[0]));
        }

		if(50 < i && i < 300){
            ROILaneEnd = frameFinalDuplicate2(Rect(i, 0, 1, 50));
            divide(255, ROILaneEnd, ROILaneEnd);
            histrogramLaneEnd.push_back((int)(sum(ROILaneEnd)[0]));
		}
	}

	laneEnd = sum(histrogramLaneEnd)[0];
	line(frameFinal, Point2f(int(50), 50), Point2f(int(300), 50), Scalar(255, 0, 0), 2);
	//cout << "Lane End: " << laneEnd << endl;
}

// Function find the curve lanes
void laneFinder() {

	vector<int>::iterator leftPtr;
	leftPtr = max_element(histrogramLane.begin(), histrogramLane.begin() + int(WIDTH / 2));
	leftLanePos = distance(histrogramLane.begin(), leftPtr);

	vector<int>::iterator rightPtr;
	rightPtr = max_element(histrogramLane.begin() + int(WIDTH / 2), histrogramLane.end());
	rightLanePos = distance(histrogramLane.begin(), rightPtr);

	line(frameFinal, Point2f(leftLanePos, 0), Point2f(leftLanePos, HEIGHT), Scalar(0, 255, 0), 2);
	line(frameFinal, Point2f(rightLanePos, 0), Point2f(rightLanePos, HEIGHT), Scalar(0, 255, 0), 2);
}

// Function find the center lane
void laneCenter() {

	centerLanePos = ((rightLanePos - leftLanePos) / 2) + leftLanePos;

	line(frameFinal, Point2f(frameCenter, 0), Point2f(frameCenter, HEIGHT), Scalar(255, 0, 0), 3);
	line(frameFinal, Point2f(centerLanePos, 0), Point2f(centerLanePos, HEIGHT), Scalar(0, 255, 0), 3);

	laneCurve = (frameCenter - centerLanePos);
}

// Function aim the car according to the lane curve
void selfDriving() {

    cout << "lane: " << laneCurve << endl;
	if (speed == 0) {
		// Stand = Decimal 0
		digitalWrite(21, 0);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 0);
		cout << "Stand" << endl;
	}
	else if (laneCurve == 0) {
		// Forward = Decimal 8
		digitalWrite(21, 0);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		//cout << "Forward" << endl;
	}
	else if (0 < laneCurve && laneCurve < 5) {
		//  Left 1 = Decimal 9
		digitalWrite(21, 1);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		cout << "Left 1" << endl;
	}
	else if (5 <= laneCurve && laneCurve < 10) {
		// Left 2 = Decimal 10
		digitalWrite(21, 0);
		digitalWrite(22, 1);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		cout << "Left 2" << endl;
	}
	else if (10 <= laneCurve) {
		// Left 3 = Decimal 11
		digitalWrite(21, 1);
		digitalWrite(22, 1);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		cout << "Left 3 - sd" << endl;
	}
	else if (-5 < laneCurve && laneCurve < 0) {
		// Right 1 = Decimal 12
		digitalWrite(21, 0);
		digitalWrite(22, 0);
		digitalWrite(23, 1);
		digitalWrite(24, 1);
		cout << "Right 1" << endl;
	}
	else if (-10 < laneCurve && laneCurve <= -5) {
		// Right 2 = Decimal 13
		digitalWrite(21, 1);
		digitalWrite(22, 0);
		digitalWrite(23, 1);
		digitalWrite(24, 1);
		cout << "Right 2" << endl;
	}
	else if (laneCurve <= -10) {
		// Right 3 = Decimal 14
		digitalWrite(21, 0);
		digitalWrite(22, 1);
		digitalWrite(23, 1);
		digitalWrite(24, 1);
		cout << "Right 3" << endl;
	}
	else if (speed < 0) {
		// Backward = Decimal 1
		digitalWrite(21, 1);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 0);
		cout << "Backword " << endl;
	}
}

// Function turn left
void turnLeft() {

	// Turn left = Decimal 15
	digitalWrite(21, 1);
	digitalWrite(22, 1);
	digitalWrite(23, 1);
	digitalWrite(24, 1);
	cout << "Turn left 1" << endl;
}

// Function print date on the frame
void printDataOnFrame() {

	if (laneEndLimit < laneEnd) {
		ss.str(" ");
		ss.clear();
		ss << "Lane End";
		putText(frame, ss.str(), Point(10, 25), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 0), 1);
	}
	else if (laneCurve == 0) {
		ss.str(" ");
		ss.clear();
		ss << "Turn = " << laneCurve << " (Move Forward)";
		putText(frame, ss.str(), Point(10, 25), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
	}
	else if (0 < laneCurve) {
		ss.str(" ");
		ss.clear();
		ss << "Turn = " << laneCurve << " (Move Left)";
		putText(frame, ss.str(), Point(10, 25), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
	}
	else if (laneCurve < 0) {
		ss.str(" ");
		ss.clear();
		ss << "Turn = " << laneCurve << " (Move Right)";
		putText(frame, ss.str(), Point(10, 25), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
	}
}

// Function display frame on the screen
void displayWindow() {

	namedWindow("Original", WINDOW_KEEPRATIO);
	resizeWindow("Original", 360, 240);
	moveWindow("Original", 360, 100);
	imshow("Original", frame);

	namedWindow("Perspective", WINDOW_KEEPRATIO);
	resizeWindow("Perspective", 360, 240);
	moveWindow("Perspective", 750, 100);
	imshow("Perspective", framePers);

	namedWindow("Final", WINDOW_KEEPRATIO);
	resizeWindow("Final", 360, 240);
	moveWindow("Final", 1150, 100);
	imshow("Final", frameFinal);

    //namedWindow("Stop Sign", WINDOW_KEEPRATIO);
	//resizeWindow("Stop Sign", 360, 240);
	//moveWindow("Stop Sign", 400, 400);
	//imshow("Stop Sign", ROI_stop);

	//namedWindow("Obstacle", WINDOW_KEEPRATIO);
	//resizeWindow("Obstacle", 360, 240);
	//moveWindow("Obstacle", 800, 400);
	//imshow("Obstacle", ROI_obstacle);
}

// Function display FPS on the screen
void getFPS() {
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	float t = elapsed_seconds.count();
	int FPS = int(1 / t);

	putText(frame, to_string(FPS), Point(5, HEIGHT - 8), FONT_HERSHEY_COMPLEX, 0.7, Scalar(255, 255, 255), 2);
}

// Function return turn value
int getInputKey() {

	int key = waitKey(1);

	if(key == 77){
        if( detectMode == true){
            detectMode = false;
            obstacleDetected = false;
        }else{
            detectMode = true;
        }
        cout << "obstacleDetected " << obstacleDetected << endl;
	}

	// key is 0
	if (key == 48) {
		// Turn left = Decimal 15
		digitalWrite(21, 1);
		digitalWrite(22, 1);
		digitalWrite(23, 1);
		digitalWrite(24, 1);

		cout << "Turn left - You pressed on the 0 key" << endl;
	}
	// key is 1 - Obstacle movement decimal 7 -- Move left
	else if (key == 49) {
		// Left 3 = Decimal 11
		digitalWrite(21, 1);
		digitalWrite(22, 1);
		digitalWrite(23, 1);
		digitalWrite(24, 0);
		cout << "Left 3 - obstacle" << endl;
	}
	// key is 2 - Move backward
	else if (key == 50) {
		// Backward = Decimal 1
		digitalWrite(21, 1);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 0);
		cout << "Backward" << endl;
	}
	// key is 3 - Move right
	else if (key == 51) {
		// Right 3 = Decimal 14
		digitalWrite(21, 0);
		digitalWrite(22, 1);
		digitalWrite(23, 1);
		digitalWrite(24, 1);
		cout << "Right 3" << endl;
	}
	// key is 4 - Move left
	else if (key == 52) {
		// Left 2 = Decimal 10
		digitalWrite(21, 0);
		digitalWrite(22, 1);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		cout << "Left 2" << endl;
	}
	// key is 5 - Stand
	else if (key == 53) {
		// Stand = Decimal 0
		digitalWrite(21, 0);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 0);
		cout << "Stand" << endl;
	}
	// key is 6 - Move right
	else if (key == 54) {
		// Right 2 = Decimal 13
		digitalWrite(21, 1);
		digitalWrite(22, 0);
		digitalWrite(23, 1);
		digitalWrite(24, 1);
		cout << "Right 2" << endl;
	}
	// key is 7 - Move left
	else if (key == 55) {
		//  Left 1 = Decimal 9
		digitalWrite(21, 1);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		cout << "Left 1" << endl;
	}
	// key is 8 - Move forward
	else if (key == 56) {
		// Forward = Decimal 8
		digitalWrite(21, 0);
		digitalWrite(22, 0);
		digitalWrite(23, 0);
		digitalWrite(24, 1);
		cout << "Forward" << endl;
	}
	// key is 9 - Move right
	else if (key == 57) {
		// Right 1 = Decimal 12
		digitalWrite(21, 0);
		digitalWrite(22, 0);
		digitalWrite(23, 1);
		digitalWrite(24, 1);
		cout << "Right 1" << endl;
	}
	return key;
}

// Function stop the car movement
void stop() {

	// Stand = Decimal 0
	digitalWrite(21, 0);
	digitalWrite(22, 0);
	digitalWrite(23, 0);
	digitalWrite(24, 0);
    cout << "Stop" << endl;
}

// Function record a video
void recordVideo(VideoWriter& video) {
	video.write(frame);
}

// Capture frames for training model
void trafficSignCapture(int countCapture) {

	Mat img;

	cvtColor(frame, img, COLOR_BGR2GRAY);
	imwrite("Stop" + to_string(countCapture) + ".jpg", img);

	namedWindow("Traffic Sign", WINDOW_KEEPRATIO);
	resizeWindow("Traffic Sign", 360, 240);
	moveWindow("Traffic Sign", 400, 400);
	imshow("Traffic Sign", img);

	waitKey();
}

// Capture negative frames for training model
void negativeCapture(int countCapture) {

	Mat img;

	cvtColor(frame, img, COLOR_BGR2GRAY);
	imwrite("Stop" + to_string(countCapture) + ".jpg", img);

	namedWindow("Traffic Sign", WINDOW_KEEPRATIO);
	resizeWindow("Traffic Sign", 360, 240);
	moveWindow("Traffic Sign", 400, 400);
	imshow("Traffic Sign", img);
}

// Stop sign detection
void Stop_detection() {

    int adjust = 1;

	frame.copyTo(frame_stop);

	vector<Rect> classified_coordinates;

	ROI_stop = frame_stop(Rect(WIDTH - int(WIDTH / 2), 0, int(WIDTH / 2), HEIGHT));
	cvtColor(ROI_stop, gray_frame_stop, COLOR_RGB2GRAY);
	//equalizeHist(gray_frame_stop, gray_frame_stop);
	Stop_Cascade.detectMultiScale(gray_frame_stop, classified_coordinates);

    // If no active tracking, initialize tracker for the first time
    if (0 < classified_coordinates.size()) {

		std::cout << "Stop sign detected! " << classified_coordinates.size() << std::endl;

        adjust = int(WIDTH / 2);

        if (trackingWindow.area() == 0) {

            // Set initial tracking window and initialize the tracker
            //tracker->init(frame, trackingWindow);  // Initialize the tracker with the adjusted position
            //std::cout << "Stop sign tracked! " << std::endl;
        }

        // Adjust coordinates to the original frame
        Rect adjustedRect(classified_coordinates[0].x + adjust, classified_coordinates[0].y, classified_coordinates[0].width, classified_coordinates[0].height);
        trackingWindow = adjustedRect;


        tracker = TrackerKCF::create();
        tracker->init(frame, trackingWindow);
        stopSignDetected = true;

        ss.str("");
        ss.clear();
        distance_stop = -0.909 * (trackingWindow.width) + (89.541); // 49X45 , 71X25
        ss << "Distance = " << distance_stop << " cm";

        rectangle(frame, trackingWindow, Scalar(0, 0, 255), 2);
        putText(frame, "Stop Sign", Point(trackingWindow.x - 12, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
        putText(frame, ss.str(), Point(trackingWindow.x - 45, trackingWindow.y + trackingWindow.height + 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
    }
	else {
		stopSignDetected = false;
		//std::cout << "No stop sign detected." << std::endl;
	}
}

// Function to track stop sign
void Track_stop_sign() {

    if (stopSignDetected) {

        bool trackingSuccess = tracker->update(frame, trackingWindow);

        if (trackingSuccess) {
            // Draw the tracking window if tracking was successful
            rectangle(frame, trackingWindow, Scalar(0, 0, 255), 2);
            putText(frame, "Stop Sign", Point(trackingWindow.x - 12, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);

			ss.str("");
			ss.clear();
			distance_stop = -0.909 * (trackingWindow.width) + (89.541); // 49X45 , 71X25

            if(selfDrivingMode == true){
                distance_counter = distance_counter +1;
                distance_stop = distance_stop - distance_counter;
            }
			//cout << distance_stop << endl;
			ss << "Distance = " << distance_stop << " cm";
			putText(frame, ss.str(), Point(trackingWindow.x - 45, trackingWindow.y + trackingWindow.height + 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
        } else {
            // Stop sign lost, reset detection
            stopSignDetected = false;
            std::cout << "Stop sign lost." << std::endl;
        }
    }
}

// Detect traffic light color light
string detectTrafficLightColor(){

	Mat frame_detection, ROI_detection;

	frame.copyTo(frame_detection);

    if (trackingWindow.x >= 0 && trackingWindow.y >= 0 &&
        trackingWindow.x + trackingWindow.width <= frame_detection.cols &&
        trackingWindow.y + trackingWindow.height <= frame_detection.rows &&
        trackingWindow.width > 0 && trackingWindow.height > 0) {

        ROI_detection = frame_detection(trackingWindow);
    } else {
        std::cerr << "Invalid tracking window!" << std::endl;
        return "Unknown";
    }
	ROI_detection = frame_detection(trackingWindow);

	Mat hsv, mask;

	// Convert the frame from BGR to HSV
	cvtColor(ROI_detection, hsv, COLOR_BGR2HSV);

	// Define HSV ranges for each color
    cv::Scalar lowerRed1(0, 100, 100), upperRed1(10, 255, 255);
    cv::Scalar lowerRed2(160, 100, 100), upperRed2(179, 255, 255);
    cv::Scalar lowerGreen(35, 100, 100), upperGreen(85, 255, 255);
    cv::Scalar lowerYellow(20, 100, 100), upperYellow(30, 255, 255);

    // Masks for each color
    cv::Mat redMask1, redMask2, redMask, greenMask, yellowMask;
    cv::inRange(hsv, lowerRed1, upperRed1, redMask1);
    cv::inRange(hsv, lowerRed2, upperRed2, redMask2);
    redMask = redMask1 | redMask2;
    cv::inRange(hsv, lowerGreen, upperGreen, greenMask);
    cv::inRange(hsv, lowerYellow, upperYellow, yellowMask);

    // Count non-zero pixels in each mask
    int redPixels = cv::countNonZero(redMask);
    int greenPixels = cv::countNonZero(greenMask);
    int yellowPixels = cv::countNonZero(yellowMask);

    cout << "Red: " << redPixels << "Green: " << greenPixels << "Yellow: " << yellowPixels << endl;

    // Determine the dominant color
    if (redPixels > greenPixels && redPixels > yellowPixels) {
        return "Red Light";
    } else if (greenPixels > redPixels && greenPixels > yellowPixels) {
        return "Green Light";
    } else if (yellowPixels > redPixels && yellowPixels > greenPixels) {
        return "Yellow Light";
    } else {
        return "Unknown"; // In case no dominant color is detected
    }
}

// Traffic Sign detection
void trafficLight_detection(){

    int adjust = 1;

	frame.copyTo(frame_trafficLight);

	vector<Rect> classified_coordinates;

	ROI_trafficLight = frame_trafficLight(Rect(WIDTH - int(WIDTH / 2), 0, int(WIDTH / 2), HEIGHT));
	cvtColor(ROI_trafficLight, gray_frame_trafficLight, COLOR_RGB2GRAY);
	//equalizeHist(gray_frame_trafficLight, gray_frame_trafficLight);
	TrafficLight_Cascade.detectMultiScale(gray_frame_trafficLight, classified_coordinates);

	//imshow("tf", ROI_trafficLight);

    // If no active tracking, initialize tracker for the first time
    if (0 < classified_coordinates.size()) {

        adjust = int(WIDTH / 2);

		std::cout << "Traffic Light detected! " << classified_coordinates.size() << std::endl;

        // Adjust coordinates to the original frame
        Rect adjustedRect(classified_coordinates[0].x + adjust - 10, classified_coordinates[0].y - 20, classified_coordinates[0].width + 20, classified_coordinates[0].height + 40 );

        trackingWindow = adjustedRect;

        if (frame.channels() == 1) {

            std::cout << "Traffic Light 1 cahnnel" << std::endl;
            std::cout << "Frame channels: " << frame.channels() << std::endl;
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        // Set initial tracking window and initialize the tracker
        tracker = TrackerKCF::create();
        tracker->init(frame, trackingWindow);

        std::cout << "Traffic Light tracking initialized!" << std::endl;

        // Detect the traffic light color
        lightColor = detectTrafficLightColor();
        std::cout << "Detected Traffic Light Color: " << lightColor << std::endl;

        cv::Scalar color;

        // Compare the string to determine the detected color
        if (lightColor == "Red Light") {
            color = cv::Scalar(0, 0, 255); // Red
            std::cout << "Stop! It's a Red Light.\n" << endl;

        } else if (lightColor == "Green Light") {
            color = cv::Scalar(0, 255, 0); // Green
            std::cout << "Go! It's a Green Light.\n" << endl;

        } else if (lightColor == "Yellow Light") {
            color = cv::Scalar(0, 255, 255); // Yellow
            std::cout << "Caution! It's a Yellow Light.\n" << endl;

        } else {
            std::cout << "Unable to determine the traffic light color.\n" << endl;
        }

        ss.str("");
        ss.clear();
        distance_trafficLight = -0.45 * (trackingWindow.width) + (55.25);
        ss << "Distance = " << distance_trafficLight << " cm";

        // Draw the tracking window and add text
        rectangle(frame, trackingWindow, color, 2);
        putText(frame, lightColor, cv::Point(trackingWindow.x -10, trackingWindow.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
        putText(frame, ss.str(), Point(trackingWindow.x - 50, trackingWindow.y + trackingWindow.height + 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);

        trafficLightDetected = true;
    }
	else {
		trafficLightDetected = false;
		//std::cout << "No traffic light detected." << std::endl;
	}
}

// Function to track stop sign
void Track_trafficLight() {

    if (trafficLightDetected) {

        cout << " " << endl;
        cout << "Track_trafficLight: " << endl;
        if (frame.channels() == 1) {

            cout << "Track_trafficLight 1 changel to 3: " << endl;
            std::cout << "Frame channels: " << frame.channels() << std::endl;
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        bool trackingSuccess = tracker->update(frame, trackingWindow);

        // Ensure tracking window is within bounds
        trackingWindow.x = std::max(0, trackingWindow.x);
        trackingWindow.y = std::max(0, trackingWindow.y);
        trackingWindow.width = std::min(frame.cols - trackingWindow.x, trackingWindow.width);
        trackingWindow.height = std::min(frame.rows - trackingWindow.y, trackingWindow.height);

        if (trackingWindow.width <= 0 || trackingWindow.height <= 0) {
            std::cerr << "Error: Invalid tracking window size!" << std::endl;
            trafficLightDetected = false;
            return;
        }

        if (trackingSuccess) {

            std::cout << "Traffic light tracking." << std::endl;

            // Detect the traffic light color
            lightColor = detectTrafficLightColor();
            std::cout << "Detected Traffic Light Color: " << lightColor << std::endl;

            cv::Scalar color;

            // Compare the string to determine the detected color
            if (lightColor == "Red Light") {
                color = cv::Scalar(0, 0, 255); // Red
                std::cout << "Stop! It's a Red Light.\n" << endl;

            } else if (lightColor == "Green Light") {
                color = cv::Scalar(0, 255, 0); // Green
                std::cout << "Go! It's a Green Light.\n" << endl;

            } else if (lightColor == "Yellow Light") {
                color = cv::Scalar(0, 255, 255); // Yellow
                std::cout << "Caution! It's a Yellow Light.\n" << endl;

            } else {
                std::cout << "Unable to determine the traffic light color.\n" << endl;
            }

			ss.str("");
			ss.clear();
			distance_trafficLight = -0.45 * (trackingWindow.width) + (55.25);
			ss << "Distance = " << distance_trafficLight << " cm";

            // Draw the tracking window and add text
            rectangle(frame, trackingWindow, color, 2);
            putText(frame, lightColor, cv::Point(trackingWindow.x -10, trackingWindow.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
			putText(frame, ss.str(), Point(trackingWindow.x - 50, trackingWindow.y + trackingWindow.height + 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);

        } else {
            // Traffic Light lost, reset detection
            trafficLightDetected = false;
            cout << " " << endl;
            std::cout << "Traffic light lost in tracking." << std::endl;
        }
    }
}

// Obtacle detection
void obstacle_detection() {

    int adjust = 1;
	frame.copyTo(frame_obstacle);
	vector<Rect> classified_coordinates;

	ROI_obstacle = frame_obstacle(Rect(int(WIDTH / 3), 50, int(WIDTH/3), HEIGHT-100));
	cvtColor(ROI_obstacle, gray_frame_obstacle, COLOR_RGB2GRAY);
	equalizeHist(gray_frame_obstacle, gray_frame_obstacle);
	Obstacle_Cascade.detectMultiScale(gray_frame_obstacle, classified_coordinates);

    //imshow("Obstacle Detection", gray_frame_obstacle);

	// Check if any stop signs were detected
	if (0 < classified_coordinates.size()) {

		//std::cout << "Obstacle detected! " << classified_coordinates.size() << std::endl;

		adjust = int(WIDTH/3);

        // Adjust coordinates to the original frame
        Rect adjustedRect(classified_coordinates[0].x + adjust,
                            classified_coordinates[0].y + 50,
                            classified_coordinates[0].width,
                            classified_coordinates[0].height);

        // Set initial tracking window and initialize the tracker
        trackingWindow = adjustedRect;

        // Adjust coordinates to the obstacle object
        Rect adjustedRect2(classified_coordinates[0].x + adjust -20,
                            classified_coordinates[0].y + 20,
                            classified_coordinates[0].width + 40,
                            classified_coordinates[0].height + 40);

        // Set initial tracking window and initialize the tracker
        trackingWindow = adjustedRect2;

        tracker = TrackerKCF::create();
        tracker->init(frame, trackingWindow);

        obstacleDetected = true;

        ss.str("");
        ss.clear();
        distance_obstacle = -2.5 * (trackingWindow.width) + (230);
        ss << "Distance = " << distance_obstacle << " cm";

        rectangle(frame, trackingWindow, Scalar(0, 0, 255), 2);
        putText(frame, "Obstacle", Point(trackingWindow.x - 2, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
        putText(frame, ss.str(), Point(trackingWindow.x - 30, trackingWindow.y + trackingWindow.height + 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
	}
	else {
		obstacleDetected = false;
		//std::cout << "No obstacle detected." << std::endl;
	}
}

// Function to track stop sign
void Track_obstacle() {

    if (obstacleDetected) {

        cout << " " << endl;
        cout << "Track_Obbstacle: " << endl;

        bool trackingSuccess = tracker->update(frame, trackingWindow);

        //cout << "Coordinates" << endl;
        //cout << trackingWindow.x << endl;
        //cout << trackingWindow.y << endl;
        //cout << trackingWindow.width << endl;

        if (trackingSuccess) {

            // Draw the tracking window if tracking was successful

			ss.str("");
			ss.clear();
			distance_obstacle = -2.5 * (trackingWindow.width) + (230);
			ss << "Distance = " << distance_obstacle << " cm";
            rectangle(frame, trackingWindow, Scalar(0, 255, 0), 2);
            putText(frame, "Tracking Obstacle", Point(trackingWindow.x - 20, trackingWindow.y -10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
			putText(frame, ss.str(), Point(trackingWindow.x - 20, trackingWindow.y + trackingWindow.height + 15), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0), 1);
        } else {
            // Obstacle lost, reset detection
            obstacleDetected = false;

            cout << " " << endl;
            cout << "Track_Obstacle feild: " << endl;
        }
    }
}

// function classify roi - 1: obstacle, 2: stop sign, traffic light
void classification_roi(Rect boundingBox, int type) {

	Mat ROI_gray;
	vector<Rect> classified_coordinates;

	// Extract the ROI and process it
	Mat ROI = frame(boundingBox).clone();
	cvtColor(ROI, ROI_gray, COLOR_RGB2GRAY);
	equalizeHist(ROI_gray, ROI_gray);

	int adjust = 1;

	// Model classification using Haar Cascade
	if (type == 1) {

		// Classify obstacle on the road
		Obstacle_Cascade.detectMultiScale(ROI_gray, classified_coordinates);

        cout << " cls1 " << classified_coordinates.size() << endl;

		// If no active tracking, initialize tracker for the first time
		if (0 < classified_coordinates.size()) {

			adjust = int(WIDTH / 3);

			if (trackingWindow.area() == 0) {

				// Adjust coordinates to the original frame
				Rect adjustedRect(classified_coordinates[0].x + adjust,
                                    classified_coordinates[0].y,
                                    classified_coordinates[0].width,
                                    classified_coordinates[0].height);

				// Set initial tracking window and initialize the tracker
				trackingWindow = adjustedRect;
				tracker->init(frame, trackingWindow);  // Initialize the tracker with the adjusted position
			}
			obstacleDetected = true;
		}
		else {
			obstacleDetected = false;
		}
	}
	else {

		// Classify stop sign on the road
		Stop_Cascade.detectMultiScale(ROI_gray, classified_coordinates);

        cout << "Stop_Cascade: " << classified_coordinates.size() <<endl;
		// If no active tracking, initialize tracker for the first time
		if (0 < classified_coordinates.size()) {

            cout << " 2" << endl;
			adjust = int(WIDTH / 2);

			if (trackingWindow.area() == 0) {

				// Adjust coordinates to the original frame
				Rect adjustedRect(classified_coordinates[0].x + adjust,
					classified_coordinates[0].y,
					classified_coordinates[0].width,
					classified_coordinates[0].height);

				// Set initial tracking window and initialize the tracker
				trackingWindow = adjustedRect;
				tracker->init(frame, trackingWindow);  // Initialize the tracker with the adjusted position
			}
			stopSignDetected = true;
		}
		else {
			stopSignDetected = false;

			// Classify traffic light on the road
			TrafficLight_Cascade.detectMultiScale(ROI_gray, classified_coordinates);

            cout << "TrafficLight_Cascade: " << classified_coordinates.size() << endl;
			if (0 < classified_coordinates.size()) {

                cout << " 4" << endl;
				adjust = int(WIDTH / 2);

				if (trackingWindow.area() == 0) {

					// Adjust coordinates to the original frame
					Rect adjustedRect(classified_coordinates[0].x + adjust,
										classified_coordinates[0].y,
										classified_coordinates[0].width,
										classified_coordinates[0].height);

					// Set initial tracking window and initialize the tracker
					trackingWindow = adjustedRect;
					tracker->init(frame, trackingWindow);  // Initialize the tracker with the adjusted position
				}
				trafficLightDetected = true;
			}
			else {

				trafficLightDetected = false;
			}

		}
	}

}

// Fuunction tracking after object
void tracking_objects(int type) {

	// Update the tracker (if it's already initialized)
	if (0 < trackingWindow.area()) {

		bool trackingSuccess = true;

		// bool trackingSuccess = tracker->update(frame, trackingWindow);

		// If tracking is successful, draw a rectangle around the object
		if (trackingSuccess) {

			rectangle(frame, trackingWindow, Scalar(0, 255, 0), 2);  // Draw green rectangle

			// stop sign
			if (type == 1) {
				// Draw the distance to the object from the camera
				distance_stop = -0.45 * (trackingWindow.width) + (40.25);

				putText(frame, "Stop sign detected", Point(trackingWindow.x, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 0), 2);  // Green text
			}
			else{

				// traffic light
				if (type == 2) {
					// Draw the distance to the object from the camera
					distance_stop = -0.45 * (trackingWindow.width) + (40.25);

					putText(frame, "traffic light detected", Point(trackingWindow.x, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 0), 2);  // Green text
				}
				else {

					// obstacle
					if (type == 3) {
						// Draw the distance to the object from the camera
						distance_stop = -0.45 * (trackingWindow.width) + (40.25);

						putText(frame, "obstacle detected", Point(trackingWindow.x, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 0), 2);  // Green text

					}
				}
			}

			ss.str("");
			ss.clear();
			ss << "Distance = " << distance_stop << " cm";
			putText(frame, ss.str(), Point(trackingWindow.x - 110, trackingWindow.y + 30), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1);
		}
		else {
			// Stop sign lost, reset detection
			obstacleDetected = false;
			stopSignDetected = false;
			trafficLightDetected = false;
			putText(frame, "Tracking failed", Point(trackingWindow.x, trackingWindow.y - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 0, 255), 2);  // Red text for failure

            cout << " " << endl;
            cout << "Tracking objects failed" << endl;
		}
	}
}

// Function detect elements
void detection_movement() {

    if( stopSignDetected == true ){

        if( 0 < distance_stop && distance_stop < 50 ){

            selfDrivingMode = false;
            Track_stop_sign();
            stop();
        }
        else{
            cout << "Stop Sign: " << distance_stop << endl;
            stopSignDetected = false;
            selfDrivingMode = true;
        }
    }

    if( obstacleDetected == true ){

        if( 0 < distance_obstacle && distance_obstacle < 100 ){

            selfDrivingMode = false;
            Track_obstacle();
            stop();
        }
        else{
            cout << "Obstacle: " << distance_obstacle << endl;
            obstacleDetected = false;
            selfDrivingMode = true;
        }
    }

    if( trafficLightDetected == true){

        if( 0 < distance_trafficLight && distance_trafficLight < 40 ){

            stop();

            Track_trafficLight();

            if(lightColor == "Red Light"){
                //selfDrivingMode = false;
                stop();
            }

            if(lightColor == "Green Light"){
                selfDrivingMode = true;
                trafficLightDetected = false;
            }

            if (lightColor == "Yellow Light") {
                stop();
            }
        }
        else{
            cout << "Traffic Light: " << distance_trafficLight << endl;
            trafficLightDetected = false;
            selfDrivingMode = true;
        }

    }
}

// Function detect shapes
void detectRectangle() {

	Mat gray, blurred, edged;

	// Extract the ROI and process it
	Mat ROI_detection = frame(Rect(WIDTH - int(WIDTH / 2), 0, int(WIDTH / 2), HEIGHT)).clone();

	// Convert to grayscale
	cvtColor(ROI_detection, gray, COLOR_BGR2GRAY);

	// Blur the image to reduce noise
	GaussianBlur(gray, blurred, Size(5, 5), 0);

	// Detect edges using Canny
	Canny(blurred, edged, 50, 150);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// Find contours
	findContours(edged, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++) {

		// Optionally, you can add bounding boxes
		Rect boundingBox = boundingRect(contours[i]);

		// Calculate the area of the bounding box
		int area = boundingBox.width * boundingBox.height;

		int minSize = 500;
		int maxSize = 7000;

		boundingBox.x = boundingBox.x + int(WIDTH / 2);

        cout << "area: " << area << endl;

		// Filter by size
		if (minSize <= area && area <= maxSize) {

			// Draw contours directly
			rectangle(frame, boundingBox, Scalar(255, 0, 0), 2);
			putText(frame, "Shape", boundingBox.tl(), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);

			// Send ROI to classification model
			classification_roi(boundingBox, 2);

			if(stopSignDetected || trafficLightDetected ){
                break;
			}
		}

	}

}

// Detect color - yellow
void detectYellow() {

	Mat frame_detection, ROI_detection;

	frame.copyTo(frame_detection);

	ROI_detection = frame_detection(Rect(int(WIDTH / 3), 0, int(WIDTH / 3), HEIGHT - 50));

	imshow("ROI_detection", ROI_detection);

	Mat hsv, mask;

	// Convert the frame from BGR to HSV
	cvtColor(ROI_detection, hsv, COLOR_BGR2HSV);

	// Define the range for yellow color in HSV
	Scalar lowerYellow(20, 100, 100); // Lower bound (Hue, Saturation, Value)
	Scalar upperYellow(30, 255, 255); // Upper bound (Hue, Saturation, Value)

	// Create a mask that isolates yellow regions
	inRange(hsv, lowerYellow, upperYellow, mask);

	// Find contours in the mask
	vector<vector<Point>> contours;
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++) {

        int adjust = int(WIDTH / 3);

		Rect boundingBox = boundingRect(contours[i]);

		boundingBox.x = boundingBox.x + adjust;

        cout << "BoundingBox " << boundingBox.area() << endl;

		// Only draw if area is larger than 500 pixels
		if (50 < boundingBox.area() && boundingBox.area() < 5000) {

			// Draw the bounding rectangle around the yellow region
			rectangle(frame, boundingBox, Scalar(255, 0, 0), 2);

            if (trackingWindow.area() == 0) {

                // Adjust coordinates to the original frame
                Rect adjustedRect(boundingBox.x - 20,
                                    boundingBox.y - 20,
                                    boundingBox.width + 40,
                                    boundingBox.height + 40);

                // Set initial tracking window and initialize the tracker
                trackingWindow = adjustedRect;
                tracker->init(frame, trackingWindow);  // Initialize the tracker with the adjusted position
                std::cout << "Obstacle tracked! " << std::endl;
            }
			// Send ROI to classification model
			//classification_roi(boundingBox, 1);
            obstacleDetected = true;
		}
	}

	// Optional: Show the mask for debugging
	imshow("Yellow Mask", mask);
}

// main setup
void main_setup(){

	cout << endl << " " << endl;
	cout << endl << "Start program" << endl;
	cout << endl << " " << endl;
    cout << " " << endl;

	VideoWriter video("out.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 50, Size(WIDTH, HEIGHT), true);

	wiringPiSetup();
	pinMode(21, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);

	//buildBar();

	setCamera(camera);

	if (!Stop_Cascade.load("StopSign/Stop_cascade.xml")) {
		printf("Unable to load cascade");
        cout << " " << endl;
	}
	else{
		printf("Stop Sign cascade loaded");
        cout << " " << endl;

	}
	if (!Obstacle_Cascade.load("Obstacle/Obstacle_cascade.xml")) {
		printf("Unable to load cascade");
        cout << " " << endl;
	}
	else{
		printf("Obstacle cascade loaded");
        cout << " " << endl;

	}
	if (!TrafficLight_Cascade.load("TrafficLight/TrafficLight_cascade.xml")) {
		printf("Unable to load cascade");
        cout << " " << endl;
	}
	else{
		printf("Traffic Light cascade loaded");
        cout << " " << endl;
	}
	cout << " " << endl;
}

int main() {

    main_setup();

	int captureFlag = 0;
	int countCapture = 1;
	int limitCapture = 40;
	int captureNagetive = 0;

    VideoWriter video("out.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 50, Size(WIDTH, HEIGHT), true);

	while (true) {

		start = std::chrono::system_clock::now();

		captureFrames();

		// Function use to train model
		if (captureFlag == 1) {

			countCapture = countCapture + 1;

			trafficSignCapture(countCapture);

			if (countCapture == limitCapture) {
				captureFlag = 0;
				countCapture = 0;
			}
		}
		if (captureNagetive == 1) {

			countCapture = countCapture + 1;

			if ((countCapture % 10) == 0) {
				negativeCapture(countCapture);
			}
		}


		if(selfDrivingMode == true){

            // Need to improve and finish
            if (stopSignDetected == false && trafficLightDetected == false && obstacleDetected == false) {

                Stop_detection();

                if ( stopSignDetected == false ){
                    trafficLight_detection();

                    if( trafficLightDetected == false && detectMode == true ){
                        obstacle_detection();
                    }
                }
            }
            else{
                detection_movement();
            }
		}
		else{
            detection_movement();
		}

		//barPerspective(); // use to initial bar values (masks)
		Perspective();

		// Define treshold value
		threshold();

		// Self-Driving algorithms
		//get_curve_direction(frame, frameFinalDuplicate, 1);
		histrogram();
		laneFinder();
		laneCenter();

		// Print data on screen
		printDataOnFrame();
		getFPS();

        //cout << "Lane End = " << laneEnd << endl;

		// Get input from user
		int inputKey = getInputKey();
		if (inputKey == 27) {
			break;
		}
		else if (inputKey == 53) { // 53 = 5 key
			selfDrivingMode = false;
		}
		else if (inputKey == 83 or inputKey == 115) { // 115 = S key and 83 = s key
			selfDrivingMode = !selfDrivingMode;
		}
		else if (inputKey == 99) { // 99 = c key
			captureFlag = 1;
		}
		else if (inputKey == 118) { // 118 = v key
			captureNagetive = 1;
		}
		else if (inputKey == -1) {

			if (selfDrivingMode == true) {

				// handle end of lane
				if (laneEndLimit < laneEnd) {

					cout << "*** Lane End = " << laneEnd << endl;

					stop();
                    selfDrivingMode = false;
					//turnLeft();
					stop();
					//selfDriving();
					//break;
				}
				else {
					selfDriving();
				}
			}
		}

		// Display final frame on the screen
		displayWindow();

		// Handle crushing
        if(rightLanePos-leftLanePos < 2){

            cout << "Out of road " << endl;
            cout << "STOP " << endl;
            stop();
            selfDrivingMode = false;
            //break;
        }

		// Record video
		//recordVideo(video);
	}

	// Print bar valuess
	cout << " " << endl;
	cout << "thresholdV = " << thresholdV << ";" << endl;
	cout << "int top_width = " << top_width << ";" << endl;
	cout << "int top_height = " << top_height << ";" << endl;
	cout << "int bot_width = " << bot_width << ";" << endl;
	cout << "int bot_height = " << bot_height << ";" << endl;
	cout << endl << "END program" << endl;

	//pi_camera.close();
    selfDrivingMode = false;
	stop();
	return 1;
}

