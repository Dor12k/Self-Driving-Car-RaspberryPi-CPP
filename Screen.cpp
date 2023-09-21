
# include "Screen.h"



// Function print fps on the screen
void Screen::design_frame(Mat &image, int fps) {

	string txt = to_string(fps);

	putText(image, txt, Point(460, 335), FONT_HERSHEY_COMPLEX, 0.7, Scalar(255, 255, 255), 2);
}

// Function display image on the screen
void Screen::display_frame(Mat image) {
	imshow("Frame", image);
}
