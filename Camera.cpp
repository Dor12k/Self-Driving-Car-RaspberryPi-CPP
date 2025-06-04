#include "Camera.h"

// Default constructor
Camera::Camera()
{
}

// Init constructor
Camera::Camera(int id) {

	this->camera.set(CAP_PROP_FRAME_HEIGHT, this->HEIGHT);
	this->camera.set(CAP_PROP_FRAME_WIDTH, this->WIDTH);
	this->camera.set(CAP_PROP_SATURATION, 50);
	this->camera.set(CAP_PROP_BRIGHTNESS, 50);
	this->camera.set(CAP_PROP_CONTRAST, 50);
	this->camera.set(CAP_PROP_FPS, 0);
	this->camera.set(CAP_PROP_GAIN, 50);

	if (id == 0) {
		camera.open(0);
	}
	if (id == 1) {
		camera.open(1);
	}
	if (id == 2) {
		camera.open(this->path);
	}
}

// Fucntion read frames from camera
Mat Camera::read_frames() {

	this->fps += 1;

	camera.read(img);

	if (img.empty()) {

		return Mat::zeros(Size(int(this->WIDTH), int(this->HEIGHT)), CV_8UC3);
	}

	return img;
}

// Function return frames per second
int Camera::get_fps() {

	clock_t end = clock();

	double timer = ((double)(end - this->start)) / CLOCKS_PER_SEC;
	//cout << timer << endl;

	if (1 < timer) {
		start = clock();
		this->FPS = this->fps;
		this->set_fps(0);
	}
	return this->FPS;
}

// Function initialize fps
void Camera::set_fps(int fps) {
	this->fps = fps;
}
