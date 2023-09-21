#include "RemoteControl.h"



// Function get key input from user
int RemoteControl::get_user_input() {

	//int key = waitKey(1) & 0xFF;
	int key = waitKey(1);

	if (key == 49 or key == 50 or key == 51 or key == 52 or key == 53 or key == 54 or key == 55 or key == 56 or key == 57 or key == 58 or key == 59) {
		return key;
	}
	else {
		if (key == 27 or key == 32) {
			return this->system_functional(key);
		}
		else {
			if (key == 83 or key == 115) {
				return key;
			}
			return -1;
		}
	}
}

// Function 
int RemoteControl::system_functional(int key) {

	if (key == 27) {
		return 27;
	}
	else {
		return -1;
	}
}

// Function turn on/off self-driving mode
bool RemoteControl::self_driving(int key, bool self_driving) {

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

// Function turn system on/off 
bool RemoteControl::system_on(int key, bool system_on) {

	// Finish the program
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

	// key is 0
	if (key == 48) {
		cout << key << endl;
	}
	// key is 1
	if (key == 49) {
		cout << key << endl;
	}
	// key is 2
	if (key == 50) {

		// Move backward
		return 0.f;
	}
	// key is 3
	if (key == 51) {
		cout << key << endl;
	}
	// key is 4
	if (key == 52) {
		
		// Move left
		return -0.2f;
	}
	// key is 5
	if (key == 53) {

		// Stand
		return 0.f;
	}
	// key is 6
	if (key == 54) {

		// Move right
		return 0.2f;
	}
	// key is 7
	if (key == 55) {
		cout << key << endl;
	}
	// key is 8
	if (key == 56) {

		// Move forward
		return 0.f;
	}
	// key is 9
	if (key == 57) {
		cout << key << endl;
	}

	return 0.f;
}

// Function return speed value
float RemoteControl::get_speed(int key) {

	// key is 0
	if (key == 48) {
		cout << key << endl;
	}
	// key is 1
	if (key == 49) {
		cout << key << endl;
	}
	// key is 2
	if (key == 50) {

		// Move backward
		return -0.5f;
	}
	// key is 3
	if (key == 51) {
		cout << key << endl;
	}
	// key is 4
	if (key == 52) {

		// Move left
		return 0.5f;
	}
	// key is 5
	if (key == 53) {

		// Stand
		return 0;
	}
	// key is 6
	if (key == 54) {

		// Move right
		return 0.5f;
	}
	// key is 7
	if (key == 55) {
		cout << key << endl;
	}
	// key is 8
	if (key == 56) {
		cout << "8" << endl;
		// Move forward
		return 0.5f;
	}
	// key is 9
	if (key == 57) {
		cout << key << endl;
	}

	return 0.f;
}