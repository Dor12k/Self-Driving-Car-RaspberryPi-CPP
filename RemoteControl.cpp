
#include "RemoteControl.h"

// Function get input key from user
int RemoteControl::get_user_input() {

	//int key = waitKey(1) & 0xFF;
	int key = waitKey(1);

	// No key pressed
	if (key == -1) {
		return -1;
	}
	else {
		// Keys 0-9 
		if (key == 49 or key == 50 or key == 51 or key == 52 or key == 53 or 
			key == 54 or key == 55 or key == 56 or key == 57 or key == 58 or key == 59) {
			return key;
		}
		return key;
	}
}

// Function turn on/off self-driving mode
bool RemoteControl::self_driving(int key, bool self_driving) {

	// 115 = S key and 83 = s key
	if (key == 83 or key == 115) {
		if (self_driving == true) {
			self_driving = false;
		}
		else {
			self_driving = true;
		}
	}
	return self_driving;
}

// Function turn on/off the program 
bool RemoteControl::system_on(int key, bool system_on) {

	// 27 = ESC
	if (key == 27) {
		if (system_on == true) {
			system_on = false;
		}
		else {
			system_on = true;
		}
	}
	return system_on;
}

// Function return turn value
float RemoteControl::get_turn(int key) {

	// No key has been pressed
	if (key == -1) {
		return 0.0f;
	}
	// key is 0
	if (key == 48) {
		cout << "You pressed on the 0 key" << endl;
	}
	// key is 1 - Move left
	if (key == 49) {
		return -0.3f;
	}
	// key is 2 - Move backward
	if (key == 50) {
		return -0.0f;
	}
	// key is 3 - Move right
	if (key == 51) {
		return 0.3f;
	}
	// key is 4 - Move left
	if (key == 52) {
		return -0.2f;
	}
	// key is 5 - Stand
	if (key == 53) {
		return 0.f;
	}
	// key is 6 - Move right
	if (key == 54) {
		return 0.2f;
	}
	// key is 7 - Move left
	if (key == 55) {
		return -0.1f;
	}
	// key is 8 - Move forward
	if (key == 56) {
		return 0.0f;
	}
	// key is 9 - Move right
	if (key == 57) {
		return 0.1f;
	}
	return 0.f;
}

// Function return speed value
float RemoteControl::get_speed(int key) {

	// No key has been pressed
	if (key == -1) {
		return 0.0f;
	}
	// key is 0
	if (key == 48) {
		cout << "You pressed on the 0 key" << endl;
	}
	// key is 1 - Move left
	if (key == 49) {
		return 0.5f;
	}
	// key is 2 - Move backward
	if (key == 50) {
		return -0.5f;
	}
	// key is 3 - Move right
	if (key == 51) {
		return 0.5f;
	}
	// key is 4 - Move left
	if (key == 52) {
		return 0.5f;
	}
	// key is 5 - Stand
	if (key == 53) {
		return 0;
	}
	// key is 6 - Move right
	if (key == 54) {
		return 0.5f;
	}
	// key is 7 - Move left
	if (key == 55) {
		return 0.5f;
	}
	// key is 8 - Move forward
	if (key == 56) {
		return 0.5f;
	}
	// key is 9 - Move right
	if (key == 57) {
		return 0.5f;
	}
	return 0.f;
}