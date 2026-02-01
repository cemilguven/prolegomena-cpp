#include <iostream>

int main()

{
	double resistance{ 5.5 };
	double current{ 10.2 };
	double voltage{ 0 };

	voltage = current * resistance;

	std::cout << "Resistance: " << resistance << " ohms\n";
	std::cout << "Current: " << current << " A\n";
	std::cout << "Voltage: " << voltage << " V\n";

	return 0;
}
