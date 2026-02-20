#include <iostream>
#include <pigpio.h>

int angle_pulse(int angle) {
	if (angle < 0) angle = 0;
	if (angle > 180) angle = 180;

	return 520 + ((2000 * angle) / 180); // to avoid decimals??
}

int main() {
	if (gpioInitialise() < 0) { // has to check for hardware failure first
		std::cout << "Failure" << '\n';
		return 1;
	}

	int gpio_pin = 12; // 12/13
	gpioSetPWMfrequency(gpio_pin, 333);
	std::cout << "Set to 333hz" << '\n';

	int target_angle = 0;

	while (true) {
		std::cout << "Enter the angle or type -1 to exit:";
		std::cin >> target_angle;
		if (target_angle == -1) {
			break;
		}
		int pulse_width_us = angle_pulse(target_angle);
		gpioServo(gpio_pin, pulse_width_us);
		std::cout << "Executed: " << target_angle << " deg -> " << pulse_width_us << " us @ 333Hz" << '\n';
	}
	gpioServo(gpio_pin, 0);
	gpioTerminate();

	return 0;

}
