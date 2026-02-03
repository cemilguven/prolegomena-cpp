#include <iostream>
#include <string>

void clear() {
	std::cout << "\033[2J\033[H" << std::flush;
}

int main() {

	int key, choice1;
	bool keepRunning = true;

	std::string password;
	std::string userInput;

	do {

		std::cout << "------------------------" << '\n';
		std::cout << "------LITTLE SIGABA-----" << '\n';
		std::cout << "------------------------" << '\n';
		std::cout << '\n';
		std::cout << "1. Encrypt Password" << '\n';
		std::cout << "\n2. Decrypt Password" << '\n';
		std::cout << '\n';
		std::cin >> choice1;

		if (choice1 == 1) {

			clear();
			std::cout << "------------------------" << '\n';
			std::cout << "---ENCRYPTION TERMINAL--" << '\n';
			std::cout << "------------------------" << '\n';
			std::cout << "\nPassword:";
			std::cin >> password;
			std::cout << "\nSecret Key:";

			std::cin >> key;

			for (int i = 0; i < password.length(); i++) {
				password[i] = password[i] + key;
			}
			std::cout << "\nEncrypted Password: " <<password;

		}

		else if (choice1 == 2) {

			clear();
			std::cout << "------------------------" << '\n';
			std::cout << "---DECRYPTION TERMINAL--" << '\n';
			std::cout << "------------------------" << '\n';
			std::cout << '\n' << "Encrypted Password:";
			std::cin >> password;
			std::cout << "\nSecret Key:";
			std::cin >> key;

			for (int i = 0; i < password.length(); i++) {
				password[i] = password[i] - key;
			}

			std::cout << "\nDecrypted Password:" <<password;

		}
		std::cout << '\n';
		std::cout << "\nWould you like to go back to the main menu? (Yes or No): ";
		std::cin >> userInput;
		clear();

		if (userInput == "yes" || userInput == "y" || userInput == "Yes" || userInput == "YES") {
			keepRunning = true;
			clear();
		}
		else if (userInput == "no" || userInput == "n" || userInput == "No" || userInput == "NO") {
			keepRunning = false;
			std::cout << "Bye!\n";
		}

	} while (keepRunning);
	return 0;

	}
