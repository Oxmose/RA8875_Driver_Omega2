#include "DigitalOut.h"
#include <iostream>
int main()
{
	DigitalOut LedPin(0);
        DigitalOut LedPin2(1);
	bool status = true;
	while(true)
	{
		std::cout << "OK" << std::endl;
		LedPin2 = status;
		LedPin = LedPin2;
		status = !status;
		usleep(1000 * 500);
	}
	return 0;
}
