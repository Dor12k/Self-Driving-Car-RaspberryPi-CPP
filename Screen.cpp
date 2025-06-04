
# include "Screen.h"

// Function print fps on the screen
void Screen::design_frame(Mat &image, int fps, float speed) {

	string FPS = to_string(fps);
	string spd = to_string(int(speed * 100)) + "cm/s";

	putText(image, FPS, Point(460, 335), FONT_HERSHEY_COMPLEX, 0.7, Scalar(255, 255, 255), 2);
	putText(image, spd, Point(5, 335), FONT_HERSHEY_COMPLEX, 0.7, Scalar(255, 255, 255), 2);
}

// Function display image on the screen
void Screen::display_frame(Mat image) {
	imshow("Frame", image);
}
