#include <iostream>
#include <cstdlib> 
#include <string>
#include <vector>

std::vector<double> history;

void clearscreen() {
	std::cout << "\033[2J\033[H" << std::flush;
}

void goback() {
	std::cout << "\nPress [Enter] to go back to menu...";
	std::cin.ignore();
	std::cin.get();
	clearscreen();
}

void circuit_analysis_menu();

void matrix_calculator_menu();

double calculate_power(double v, double i) {
	return v * i;
}

double calculate_voltage(double i, double r) {
	return i * r;
}

double calculate_current(double v, double r) {
	return v / r;
}

double calculate_resistance(double v, double i) {
	return v / i;
}

double calculate_power_from_voltage_resistance(double v, double r) {
	return (v * v) / r;
}

double calculate_power_from_current_resistance(double i, double r) {
	return (i * i) * r;
}

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
		int main_menu_choice;
		std::cin >> main_menu_choice;
		clearscreen();

		if (!(main_menu_choice)) {
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				clearscreen();
				std::cout << "Invalid Input." << '\n';
				std::cout << "\n";
				continue;
		}

		if (main_menu_choice == 1) {
			circuit_analysis_menu();
		}

		else if (main_menu_choice == 2) {
			matrix_calculator_menu();
		}

		else if (main_menu_choice == 3) {
			clearscreen();
			std::cout << "--- Calculation History ---" << '\n';
			std::cout << '\n';
			if (history.empty()) {
				std::cout << "No history yet.\n";
				goback();

			}
			else {
				for (double n : history) {
					std::cout << n << '\n';
				}
				goback();
			}

		}

		else if (main_menu_choice == 4) {
			main_menu = false;
			break;
		}

		else {
			clearscreen();
			std::cout << "Invalid Input." << '\n';
			std::cout << "\n";
		}
	}
}

void circuit_analysis_menu() {

	bool circuit_analysis_menu = true;

	while (circuit_analysis_menu) {

		int circuit_menu_choice;

		std::cout << "---------------------------------\n";
		std::cout << "---CIRCUIT ANALYSIS CALCULATOR---\n";
		std::cout << "---------------------------------\n";
		std::cout << "\n1. Ohm's Law Calculator\n";
		std::cout << "\n2. Power Calculator\n";
		std::cout << "\n3. Go Back\n";
		std::cout << "\n";
		std::cin >> circuit_menu_choice;
		clearscreen();

		if (!(circuit_menu_choice)) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			clearscreen();
			std::cout << "Invalid Input." << '\n';
			std::cout << "\n";
			continue;
		}

		if (circuit_menu_choice == 1) {

			int ohm_menu_choice;

			std::cout << "1. Voltage Calculator [I * R]" << '\n';
			std::cout << "\n2. Current Calculator [V / R]" << '\n';
			std::cout << "\n3. Resistance Calculator [V / I]" << '\n';
			std::cout << "\n";
			std::cin >> ohm_menu_choice;


			clearscreen();
			if (ohm_menu_choice == 1) {
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

			else if (ohm_menu_choice == 2) {
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

			else if (ohm_menu_choice == 3) {
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

		else if (circuit_menu_choice == 2) {

			int power_menu_choice;

			std::cout << "1. Using Voltage and Resistance [V^2 / R] " << '\n';
			std::cout << "\n2. Using Current and Resistance [I^2 * R] " << '\n';
			std::cout << "\n3. Using Voltage and Current [V * I]" << '\n';
			std::cout << "\n";
			std::cin >> power_menu_choice;

			if (!(power_menu_choice)) {
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				clearscreen();
				std::cout << "Invalid Input." << '\n';
				std::cout << "\n";
				continue;
			}

			if (power_menu_choice == 1) {
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

			else if (power_menu_choice == 2) {
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
			else if (power_menu_choice == 3) {
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

		else if (circuit_menu_choice == 3) {

			circuit_analysis_menu = false;
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

void matrix_calculator_menu() {

	int matrix_menu_choice;

	bool matrix_calculator_menu = true;

	int rows, columns;

	while (matrix_calculator_menu) {

			std::cout << "---------------------------------" << '\n';
			std::cout << "--------MATRIX CALCULATOR--------" << '\n';
			std::cout << "---------------------------------" << '\n';
			std::cout << "\n1. Matrix Addition" << '\n';
			std::cout << "\n2. Matrix Subtraction" << '\n';
			std::cout << "\n3. Matrix Multiplication" << '\n';
			std::cout << "\n4. Go back" << '\n';
			std::cout << '\n';
			std::cin >> matrix_menu_choice;

			if (!(matrix_menu_choice)) {
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				clearscreen();
				std::cout << "Invalid Input." << '\n';
				std::cout << "\n";
				continue;
			}

			if (matrix_menu_choice == 1) {
				clearscreen();

				std::cout << "Enter the number of rows: ";
				std::cin >> rows;
				std::cout << '\n';

				std::cout << "Enter the number of columns: ";
				std::cin >> columns;

				std::vector<std::vector<double>> first_matrix(rows, std::vector<double>(columns));
				std::vector<std::vector<double>> second_matrix(rows, std::vector<double>(columns));
				std::vector<std::vector<double>> sum_of_matrices(rows, std::vector<double>(columns));

				clearscreen();
				std::cout << "--- Enter elements of the First Matrix ---\n";
				std::cout << '\n';

				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < columns; j++) {
						std::cout << "Enter Elements [" << i + 1 << "][" << j + 1 << "]: ";
						std::cin >> first_matrix[i][j];
					}
					std::cout << '\n';
				}
				clearscreen();
				std::cout << "--- Enter elements of the Second Matrix ---\n";
				std::cout << '\n';

				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < columns; j++) {
						std::cout << "Enter Element [" << i + 1 << "][" << j + 1 << "]: ";
						std::cin >> second_matrix[i][j];
					}
					std::cout << '\n';
				}
				clearscreen();
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < columns; j++) {
						sum_of_matrices[i][j] = first_matrix[i][j] + second_matrix[i][j];
					}
				}

				clearscreen();
				std::cout << "--- Sum of Matrices ---\n";
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < columns; j++) {
						std::cout << sum_of_matrices[i][j] << "\t"; 
					}
					std::cout << "\n";
				}
				goback();
			}

			else if (matrix_menu_choice == 4) {
				matrix_calculator_menu = false;
				clearscreen();
			}

		}
}
