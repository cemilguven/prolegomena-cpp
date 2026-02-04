#include <iostream>
#include <cstdlib> 
#include <string>
#include <vector>

double calculate_voltage(double i, double r) {
	return i * r;
}

double calculate_current(double v, double r) {
	return v / r;
}

double calculate_resistance(double v, double i) {
	return v / i;
}

double calculate_power(double v, double i) {
	return v * i;
}

double calculate_power_from_voltage_resistance(double v, double r) {
	return (v * v) / r;
}

double calculate_power_from_current_resistance(double i, double r) {
	return (i * i) * r;
}

void clearscreen() {
	std::cout << "\033[2J\033[H" << std::flush;
}

void goback() {
	std::cout << "\nPress [Enter] to go back to menu...";
	std::cin.ignore();
	std::cin.get();
	clearscreen();
}

std::vector<double> history;
std::string userInput;

void circuit_analysis_menu();

int main() {

	bool main_menu = true;

	while (main_menu) {

		std::cout << "--------------------------------\n";
		std::cout << "----MULTI PURPOSE CALCULATOR----\n";
		std::cout << "--------------------------------\n";
		std::cout << "\n1. Circuit Analysis Calculator\n";
		std::cout << "\n2. Matrix Calculator\n";
		std::cout << "\n3. History\n";
		std::cout << "\n4. Exit\n";
		std::cout << "\n";
		int choice0;
		std::cin >> choice0;
		clearscreen();

			if (choice0 == 1) {
				circuit_analysis_menu();
			}

			else if (choice0 == 3) {
				std::cout << "--- Calculation History ---" << '\n';
				if (history.empty()) {
					clearscreen();
					std::cout << "No history yet.\n";
					goback();

				}
				else {
					for (double n : history) {
						std::cout << n << '\n';
						goback();
					}
				}

			}

			else if (choice0 == 4) {
				main_menu = false;
				break;
			}

			else {
				clearscreen();
				std::cout << "Invalid Input.\n";
				std::cout << "\n";
			}
		}
	}

void circuit_analysis_menu() {

	bool in_circuit_mode = true;
	int sub_choice1;
	int sub_choice2;
	int sub_choice3;
	int sub_choice0;

	while (in_circuit_mode) {

		std::cout << "-------------------------------\n";
		std::cout << "-----CIRCUIT ANALYSIS MENU-----\n";
		std::cout << "-------------------------------\n";
		std::cout << "\n1. Ohm's Law Calculator\n";
		std::cout << "\n2. Power Calculator\n";
		std::cout << "\n3. Go Back\n";
		std::cout << "\n";
		std::cin >> sub_choice0;
		clearscreen();

		if (sub_choice0 == 1) {

			std::cout << "1. Voltage Calculator [I * R]" << '\n';
			std::cout << "\n2. Current Calculator [V / R]" << '\n';
			std::cout << "\n3. Resistance Calculator [V / I]" << '\n';
			std::cout << "\n";
			std::cin >> sub_choice1;


			clearscreen();
			if (sub_choice1 == 1) {
				double i, r;

				std::cout << "Enter Current (A): ";
				std::cin >> i;
				std::cout << '\n';

				std::cout << "Enter Resistance (ohms): ";
				std::cin >> r;
				std::cout << '\n';

				double result = calculate_voltage(i, r);
				std::cout << "Voltage: " << result << " V" << '\n';
				history.push_back(result);
				goback();
			}

			else if (sub_choice1 == 2) {
				double v, r;

				std::cout << "Enter Voltage (V): ";
				std::cin >> v;
				std::cout << '\n';

				std::cout << "Enter Resistance (ohms): ";
				std::cin >> r;
				std::cout << '\n';

				double result = calculate_current(v, r);
				std::cout << "Current: " << result << " A" << '\n';
				history.push_back(result);
				goback();
			}

			else if (sub_choice1 == 3) {
				double v, i;

				std::cout << "Enter Voltage (V): ";
				std::cin >> v;
				std::cout << '\n';

				std::cout << "Enter Current (A): ";
				std::cin >> i;
				std::cout << '\n';

				double result = calculate_resistance(v, i);
				std::cout << "Resistance: " << result << " ohms" << '\n';
				history.push_back(result);
				goback();

			}

		}

		else if (sub_choice0 == 2) {

			std::cout << "1. Using Voltage and Resistance [V^2 / R] " << '\n';
			std::cout << "\n2. Using Current and Resistance [I^2 * R] " << '\n';
			std::cout << "\n3. Using Voltage and Current [V * I]" << '\n';
			std::cout << "\n";
			std::cin >> sub_choice3;



			if (sub_choice3 == 1) {
				double v, r;

				clearscreen();

				std::cout << "Enter Voltage (V): ";
				std::cin >> v;
				std::cout << '\n';

				std::cout << "Enter Resistance (ohms): ";
				std::cin >> r;
				std::cout << '\n';

				double result = calculate_power_from_voltage_resistance(v, r);
				std::cout << "Power: " << result << " W" << '\n';
				history.push_back(result);
				goback();
			}

			else if (sub_choice3 == 2) {
				double i, r;

				clearscreen();

				std::cout << "Enter Current (A): ";
				std::cin >> i;
				std::cout << '\n';

				std::cout << "Enter Resistance (ohms): ";
				std::cin >> r;
				std::cout << '\n';

				double result = calculate_power_from_current_resistance(i, r);
				std::cout << "Power: " << result << " W" << '\n';
				history.push_back(result);
				goback();

			}
			else if (sub_choice3 == 3) {
				double v, i;

				clearscreen();

				std::cout << "Enter Voltage (V): ";
				std::cin >> v;
				std::cout << '\n';

				std::cout << "Enter Current (A): ";
				std::cin >> i;
				std::cout << '\n';

				double result = calculate_power(v, i);
				std::cout << "Power: " << result << " W" << '\n';
				history.push_back(result);
				goback();
			}
		}

			else if (sub_choice0 == 3) {

				in_circuit_mode = false;
				clearscreen();
			}

			else {
				clearscreen();
				std::cout << "Invalid Input.\n";
				std::cout << "\n";
				goback();
			}

	}

	}
