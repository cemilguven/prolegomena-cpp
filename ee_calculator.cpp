#include <iostream>
#include <cstdlib> 
#include <string>

int main()

{

	bool keepRunning = true;
	std::string userInput;
	do {

		int choice;
		int choice2;

		double resistance;
		double current;
		double voltage;
		double power;

		std::cout << "\n1. Ohm's Law Calculator\n";
		std::cout << "\n2. Power Calculator\n";
		std::cout << "\n3. Test\n";
		std::cout << "\n";
		std::cin >> choice;


		system("cls");

		if (choice == 1) {

			std::cout << "1. Voltage Calculator\n";
			std::cout << "\n2. Current Calculator\n";
			std::cout << "\n3. Resistance Calculator\n";
			std::cout << "\n";
			std::cin >> choice2;


			system("cls");

			if (choice2 == 1) {

				std::cout << "Enter Current (Amps): ";
				std::cin >> current;

				std::cout << "Enter Resistance (ohms): ";
				std::cin >> resistance;

				voltage = current * resistance;

				std::cout << "Calculated Voltage is: " << voltage << "V" << '\n';
			}


			else if (choice2 == 2) {

				std::cout << "Enter Voltage (V): ";
				std::cin >> voltage;

				std::cout << "Enter Resistance (ohms): ";
				std::cin >> resistance;

				current = voltage / resistance;

				std::cout << "Calculated Current is: " << current << "A" << '\n';
			}

			else if (choice2 == 3) {

				std::cout << "Enter Voltage (V): ";
				std::cin >> voltage;

				std::cout << "Enter Current (A): ";
				std::cin >> current;

				resistance = voltage / current;

				std::cout << "Calculated Resistance is: " << resistance << " ohms" << '\n';

			}

			else {
				std::cout << "Invalid Input.\n";
			}

		}

		else if (choice == 2) {

			std::cout << "Enter Voltage (V): ";
			std::cin >> voltage;

			std::cout << "Enter Current (A): ";
			std::cin >> current;

			power = voltage * current;

			std::cout << "Calculated Power is: " << power << " W" << '\n';
		}

		std::cout << "\nWould you like to continue? (Yes or No): ";
		std::cin >> userInput;

		if (userInput == "yes" || userInput == "y" || userInput == "Yes" || userInput == "YES") {
			keepRunning = true;
			system("cls");
		}
		else if (userInput == "no" || userInput == "n" || userInput == "No" || userInput == "NO") {
			keepRunning = false;
			std::cout << "Bye!\n";
		}

		else {
			std::cout << "Invalid Input.\n";
		}

	} while (keepRunning);
	return 0;

}
