//double check DONT FORGET MIN MAX TEST CHANGE ALL VALUES IN FILES
//double check DONT FORGET MIN MAX TEST CHANGE ALL VALUES IN FILES

// DO CHRONO TELEMETRY CHECK PAST MAINS !!!!!!

#include "argus_vision.h"
#include "argus_core.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include <pigpio.h>  

constexpr int gpio_x = 12;
constexpr int gpio_y = 13;

enum ArgusState {

	IDLE,
	SCAN,
	TRACK,
	LOCKED,
};

int main() {

	std::cout << "[ARGUS] INITIALIZING..." << '\n';

	ControlState pid_core;
	ArgusState current_state = IDLE;

	if (gpioInitialise() < 0) {
		std::cerr << "[ARGUS] PIGPIO COULD NOT BE INITIALIZED" << '\n'; //if crashes cout is too late
		return -1;
	}

	cv::VideoCapture cap(0);

	if (!cap.isOpened()) {
		std::cerr << "[ARGUS] CAMERA COULDD NOT BE INITIALIZED" << '\n';
		gpioTerminate();
		return -1;
	}

	gpioServo(gpio_x, 1365);
	gpioServo(gpio_y, 1365);

	int min_hue = 0, max_hue = 179; //test
	int min_sat = 0, max_sat = 255;
	int min_val = 0, max_val = 255;

	auto start_time = std::chrono::steady_clock::now();
	auto last_time = start_time;

	std::cout << "[ARGUS] ONLINE" << '\n';

	while (true) {

		auto current_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = current_time - last_time;
		double dt = diff.count();
		last_time = current_time;

		VisionState vision_data = processFrame(cap, min_hue, max_hue, min_sat, max_sat, min_val, max_val);

		switch (current_state) {

		case IDLE:

			gpioServo(gpio_x, 1365); //double check MIN MAX WRONG DONT FORGET MIN MAX TEST CHANGE ALL VALUES IN FILES
			gpioServo(gpio_y, 1365);

			if (vision_data.laser_detected) {

				std::cout << "[ARGUS] LASER DETECTED.." << '\n';

				current_state = SCAN;
			}
			break;

		case SCAN:

			if (vision_data.target_detected && vision_data.laser_detected) {

				std::cout << "[ARGUS] TARGET ACQUIRED.." << '\n';

				current_state = TRACK;

			}
			else {
				gpioServo(gpio_x, 1365);
				gpioServo(gpio_y, 1365);
			}
			break;

		case TRACK:
			if (!vision_data.target_detected) {

				std::cout << "[ARGUS] TARGET LOST" << '\n';

				current_state = SCAN;
			}

			else if (!vision_data.laser_detected) {

				std::cout << "[ARGUS] LASER LOST" << '\n';

				current_state = SCAN;
			}

			else if (vision_data.locked) {

				std::cout << "[ARGUS] LOCKED.." << '\n';

				current_state = LOCKED;
			}

			else {

				float pwm_out_x = 0;
				float pwm_out_y = 0;

				CoreControl(pid_core, vision_data.target_x, vision_data.target_y, vision_data.laser_x, vision_data.laser_y, dt, pwm_out_x, pwm_out_y);

				gpioServo(gpio_x, static_cast<int>(pid_core.pwm_x));
				gpioServo(gpio_y static_cast<int>(pid_core.pwm_y));
			}
			break;

		case LOCKED:

			if (!vision_data.locked) {
				std::cout << "[ARGUS] TARGET LOCK LOST" << '\n';

				current_state = TRACK;
			}

			else {
				float pwm_out_x = 0;
				float pwm_out_y = 0;

				CoreControl(pid_core, vision_data.target_x, vision_data.target_y, vision_data.laser_x, vision_data.laser_y, dt, pwm_out_x, pwm_out_y);

				gpioServo(gpio_x, static_cast<int>(pid_core.pwm_x));
				gpioServo(gpio_y, static_cast<int>(pid_core.pwm_y));

			}
			break;
		}

		if (!vision_data.frame.empty()) {     // test performance w wo
			cv::imshow("ARGUS X-1 OV9281", vision_data.frame);
		}

		if (cv::waitKey(1) == 27) {
			std::cout << "[ARGUS] TERMINATED.." << '\n';
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();
	gpioServo(gpio_x, 0);
	gpioServo(gpio_y, 0);
	gpioTerminate();

	return 0;

}
