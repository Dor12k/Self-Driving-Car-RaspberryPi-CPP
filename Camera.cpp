#include "Camera.h"

// Default constructor
Camera::Camera()
{
}

// Init constructor
Camera::Camera(int id) {

	if (id == 0) {
		camera.open(0);
	}
	if (id == 1) {
		camera.open(1);
	}
	if (id == 2) {
		camera.open(path);
	}
}

// Fucntion read frames from camera
Mat Camera::read_frames() {

	camera.read(img);

	this->fps += 1;

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
