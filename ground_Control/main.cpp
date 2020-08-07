#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>

using namespace std;

// application reads from the specified serial port and reports the collected data
int main()
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM3");    // adjust as needed

	if (SP->IsConnected())
		printf("Successful connected!\n");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;

	char onCommand[6] = "$tuon";
	char offCommand[6] = "$tuof";
	int mode;
	

	while (SP->IsConnected())
	{
		printf("Enter the number of Command (1 is on, 2 is off) : ");
		cin >> mode;
		if (mode == 1)
			SP->WriteData(onCommand, 6);
		else
			SP->WriteData(offCommand, 6);

		readResult = SP->ReadData(incomingData, dataLength);
		// printf("Bytes read: (0 means no data available) %i\n",readResult);
		incomingData[readResult] = 0;

		printf("%s\n", incomingData);

		Sleep(500);
	}
	return 0;
}
