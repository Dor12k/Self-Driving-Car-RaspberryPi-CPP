
# include "Motor.h"

Motor::Motor(int enableA, int inA1, int inA2, int inB1, int inB2, int enableB) {

	this->enableA = enableA;
	this->inA1 = inA1;
	this->inA2 = inA2;
	this->inB1 = inB1;
	this->inB2 = inB2;
	this->enableB = enableB;

	this->movement_speed = this->motor_movement_speed * (this->motor_sdk_movement_speed / this->SDK_SPEED);

	wiringPiSetup();
	pinMode(21, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);
}

// Function find the right direction command
int Motor::driving_manager(float speed, float turn) {

	turn = turn * 100;
	speed = speed * 100.0f;

	int left_speed = this->get_left_speed(speed, turn);

	int right_speed = this->get_right_speed(speed, turn);

	int distance = int(this->movement_speed * this->interval);

	if (left_speed != 0) {
		//cout << speed << endl;
		//cout << turn << endl;
		//cout << left_speed << endl;
		//cout << right_speed << endl;
		//cout << this->last_cmd << endl;
		//cout << " " << endl;

	}

	if (speed == 0) {
		this->stand();
	}
	else {
		if (speed < 0) {

			if (turn == 0) {
				//thread backward_thread1(&Motor::move_backward, this, left_speed, right_speed, distance);
				//backward_thread1.detach();
				this->move_backward(left_speed, right_speed, distance);
			}
			else {
				if (turn < 0) {
					//thread backward_thread1(&Motor::move_reverse_left, this, left_speed, right_speed, distance);
					//backward_thread1.detach();
					this->move_reverse_left(left_speed, right_speed, distance);
				}
				else {
					if (0 < turn) {
						//thread backward_thread1(&Motor::move_reverse_right, this, left_speed, right_speed, distance);
						//backward_thread1.detach();
						this->move_reverse_right(left_speed, right_speed, distance);
					}
				}
			}
		}
		else {
			if (turn == 0) {
				//thread forward_thread1(&Motor::move_forward, this, left_speed, right_speed, distance);
				//forward_thread1.detach();
				//forward_thread1.join();
				this->move_forward(left_speed, right_speed, distance);
			}
			else {
				if (turn < 0) {
					//thread left_thread1(&Motor::move_left, this, left_speed, right_speed, distance);
					//left_thread1.detach();
					this->move_right(left_speed, right_speed, distance);
				}
				else {
					if (0 < turn) {
						//thread right_thread1(&Motor::move_right, this, left_speed, right_speed, distance);
						//right_thread1.detach();
						this->move_left(left_speed, right_speed, distance);
					}
				}
			}
		}
	}
	return 1;
}

// Function find the left speed of the motor
int Motor::get_left_speed(float speed, float turn) {

	int left_speed = (int)(speed + turn);

	// Define speed limits
	if (100 < left_speed) {
		left_speed = 100;
	}
	else {
		if (left_speed < -100) {
			left_speed = -100;
		}
	}

	left_speed = abs(left_speed);

	return left_speed;

}

// Function find the right speed of the motor
int Motor::get_right_speed(float speed, float turn) {

	int right_speed = (int)(speed - turn);

	// Define speed limits
	if (100 < right_speed) {
		right_speed = 100;
	}
	else {
		if (right_speed < -100) {
			right_speed = -100;
		}
	}

	right_speed = abs(right_speed);

	return right_speed;
}

int Motor::move_forward(int left_speed, int right_speed, int distance){

	if (this->last_cmd != "Forward") {
		this->last_cmd = "Forward";

		thread forward_thread(&Motor::motor_forward, this, left_speed, right_speed, distance);
		forward_thread.join();

		this->stand();
	}
	else {

	}
	return 1;
}

int Motor::move_backward(int left_speed, int right_speed, int distance){

	if (this->last_cmd != "Backward") {
		this->last_cmd = "Backward";

		thread backward_thread(&Motor::motor_backward, this, left_speed, right_speed, distance);
		backward_thread.join();
		this->stand();
	}
	return 1;
}

int Motor::move_right(int left_speed, int right_speed, int distance){

	if (this->last_cmd != "Right") {
		this->last_cmd = "Right";

		thread right_thread(&Motor::motor_right, this, left_speed, right_speed, distance);
		right_thread.join();

		this->stand();
	}
	else {

	}
	return 1;
}

int Motor::move_left(int left_speed, int right_speed, int distance){

	if (this->last_cmd != "Left") {
		this->last_cmd = "Left";

		thread left_thread(&Motor::motor_left, this, left_speed, right_speed, distance);
		left_thread.join();

		this->stand();
	}
	return 1;
}

int Motor::move_reverse_right(int left_speed, int right_speed, int distance) {

	if (this->last_cmd != "Reverse Right") {
		this->last_cmd = "Reverse Right";

		thread reverse_right_thread(&Motor::motor_reverse_right, this, left_speed, right_speed, distance);
		reverse_right_thread.join();

		this->stand();
	}
	return 1;
}

int Motor::move_reverse_left(int left_speed, int right_speed, int distance) {

	if (this->last_cmd != "Reverse Left") {
		this->last_cmd = "Reverse Left";

		thread reverse_left_thread(&Motor::move_reverse_left, this, left_speed, right_speed, distance);
		reverse_left_thread.join();

		this->stand();
	}
	return 1;
}

int Motor::stand() {
	if (this->last_cmd != "Stand") {

		this->last_cmd = "Stand";

		this->motor_stand();
	}
	return 1;
}

int Motor::motor_reverse_right(int left_speed, int right_speed, int distance) {

	float timer = abs(float(distance / this->movement_speed));
	this_thread::sleep_for(chrono::milliseconds(int(timer * 1000)));
	return 1;
}

int Motor::motor_reverse_left(int left_speed, int right_speed, int distance) {

	float timer = abs(float(distance / this->movement_speed));
	this_thread::sleep_for(chrono::milliseconds(int(timer * 1000)));
	return 1;
}

int Motor::motor_backward(int left_speed, int right_speed, int distance) {

	// Decimal 1
	digitalWrite(21, 1);
	digitalWrite(22, 0);
	digitalWrite(23, 0);
	digitalWrite(24, 0);

	float timer = abs(float(distance / this->movement_speed));
	this_thread::sleep_for(chrono::milliseconds(int(timer * 1000)));

	//cout << "Motor: " << endl;
	//cout << "Motor Left: Left Speed: " << left_speed << endl;
	//cout << "Motor Left: Right Speed: " << right_speed << endl;
	return 1;
}

int Motor::motor_forward(int left_speed, int right_speed, int distance) {

	// Decimal 8
	digitalWrite(21, 0);
	digitalWrite(22, 0);
	digitalWrite(23, 0);
	digitalWrite(24, 1);

	//cout << "Motor: " << endl;
	//cout << "Motor Left: Left Speed: " << left_speed << endl;
	//cout << "Motor Left: Right Speed: " << right_speed << endl;

	float timer = abs(float(distance / this->movement_speed));
	this_thread::sleep_for(chrono::milliseconds(int(timer * 1000)));
	return 1;
}

int Motor::motor_left(int left_speed, int right_speed, int distance) {


    int speed = left_speed - right_speed;

	//cout << "Motor: " << endl;
	//cout << "Motor Left: Left Speed: " << left_speed << endl;
	//cout << "Motor Left: Right Speed: " << right_speed << endl;
    cout << "Motor Left Speed: " << speed << endl;

    speed = abs(speed);

    // Need to schronize the left123 dec with arduino
    if(speed < 20){
        // Left1 = Decimal 9
        digitalWrite(21, 1);
        digitalWrite(22, 1);
        digitalWrite(23, 0);
        digitalWrite(24, 1);

    }
    else{

        if(speed <= 40){
            // Left2 = Decimal 10
            digitalWrite(21, 0);
            digitalWrite(22, 1);
            digitalWrite(23, 0);
            digitalWrite(24, 1);
        }
        else{

            if(40 < speed){
            // Left2 = Decimal 11
                digitalWrite(21, 1);
                digitalWrite(22, 0);
                digitalWrite(23, 0);
                digitalWrite(24, 1);
            }
        }
    }

	float timer = abs(float(distance / this->movement_speed));
	this_thread::sleep_for(chrono::milliseconds(int(timer * 1000)));
	return 1;
}

int Motor::motor_right(int left_speed, int right_speed, int distance) {

    int speed = left_speed - right_speed;

	//cout << "Motor: " << endl;
	//cout << "Motor Left: Left Speed: " << left_speed << endl;
	//cout << "Motor Left: Right Speed: " << right_speed << endl;
    cout << "Motor Right Speed: " << speed << endl;

    speed = abs(speed);

    // Need to schronize the right123 dec with arduino
    if(speed < 40){
    // Right1 = Decimal 12
    	digitalWrite(21, 0);
    	digitalWrite(22, 1);
    	digitalWrite(23, 1);
    	digitalWrite(24, 1);
    }
    else{
        if(speed <= 60){
            // Right2 = Decimal 13
            digitalWrite(21, 1);
            digitalWrite(22, 0);
            digitalWrite(23, 1);
            digitalWrite(24, 1);
        }
        else{
            if(80 < speed){
            // Right3 = Decimal 12
                digitalWrite(21, 0);
                digitalWrite(22, 0);
                digitalWrite(23, 1);
                digitalWrite(24, 1);
            }
        }
    }
	float timer = abs(float(distance / this->movement_speed));
	this_thread::sleep_for(chrono::milliseconds(int(timer * 1000)));
	return 1;
}

void Motor::motor_stand() {

	// Decimal 1
	digitalWrite(21, 0);
	digitalWrite(22, 0);
	digitalWrite(23, 0);
	digitalWrite(24, 0);

	//cout << "Motor: " << endl;

	this_thread::sleep_for(chrono::milliseconds(int(0.1 * 1000)));
}
