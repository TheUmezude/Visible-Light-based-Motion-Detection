#include <cstdio>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <iterator>
#include <sstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <strstream>

#include "serial_com.h"

// include this header file for basic image transfer functions
#include "image_transfer3.h"
#include "vision.h"
#include "timer.h"
#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )

//This code detects motion using vision system.

using namespace std;

int main(int argc, char* argv[])
{
	int cam_number, size, timecount, timeSeconds;
	int pixelcount1;
	int pixelcount2;
	int Sensitivity;
	int height = 480, width = 640;
	char response;
	FILE *fp;
	char fileName[100];
	image backgroundcolor1, backgroundcolor2, backgroundgrey1, backgroundgrey2;
	image color1, color2, grey1, grey2;
	image image_compare;

	ofstream outputFile;
	outputFile.open("MotionResult.txt");
	outputFile << "MOTION DETECTION UPDATE IS AS FOLLOWS:\n\n" << endl;

	image_compare.type = GREY_IMAGE;
	image_compare.width = 640;
	image_compare.height = 480;

	backgroundcolor1.type = RGB_IMAGE;
	backgroundcolor1.width = 640;
	backgroundcolor1.height = 480;

	backgroundcolor2.type = RGB_IMAGE;
	backgroundcolor2.width = 640;
	backgroundcolor2.height = 480;

	backgroundgrey1.type = GREY_IMAGE;
	backgroundgrey1.width = 640;
	backgroundgrey1.height = 480;

	backgroundgrey2.type = GREY_IMAGE;
	backgroundgrey2.width = 640;
	backgroundgrey2.height = 480;

	color1.type = RGB_IMAGE;
	color1.width = 640;
	color1.height = 480;

	color2.type = RGB_IMAGE;
	color2.width = 640;
	color2.height = 480;

	grey1.type = GREY_IMAGE;
	grey1.width = 640;
	grey1.height = 480;

	grey2.type = GREY_IMAGE;
	grey2.width = 640;
	grey2.height = 480;

	allocate_image(color1);
	allocate_image(color2);
	allocate_image(backgroundcolor1);
	allocate_image(backgroundcolor2);
	allocate_image(grey1);
	allocate_image(grey2);
	allocate_image(backgroundgrey1);
	allocate_image(backgroundgrey2);
	allocate_image(image_compare);
	
	cam_number = 0;
	activate_camera(cam_number, height, width);

	while (1)
	{
		if (KEY('X')) break;

		cout << "\n***************************************** PROGRAM MENU *****************************************\n" << endl;
		cout << "\aTo activate motion-detection, Kindly select a sensitivity option, below:" << endl;
		cout << "Select just the option, e.g., '2'... Note that results would also be stored in 'MotionResult.txt'\n" << endl;
		cout << "1. Very-High Sensitivity (Detects even the slightest motions)" << endl;
		cout << "2. High Sensitivity (Less-Sensitive than option 1; but still high sensitivity)" << endl;
		cout << "3. Medium Sensitivity (You can choose this option if you are confused)" << endl;
		cout << "4. Low Sensitivity (Doesn't detect small motions)" << endl;
		cout << "5. Very-Low Sensitivity (Less-Sensitive than option 4)" << endl;
		//cout << "\nPress Key 'X' on the keyboard, to exit the program.";
		cout << "\nYour Response: ";
		cin >> response;

		if (response == '1'){
			Sensitivity = 500;
			cout << "\n**** You selected 'Very-High Sensitivty'. ****" << endl;
			outputFile << "\n**** You selected 'Very-High Sensitivty'. ****" << endl;
		}
		else if (response == '2') {
			Sensitivity = 2000;
			cout << "\n**** You selected 'High Sensitivty'. ****" << endl;
			outputFile << "\n**** You selected 'High Sensitivty'. ****" << endl;
		}
		else if (response == '3') {
			Sensitivity = 4500;
			cout << "\n**** You selected 'Medium Sensitivty'. ****" << endl;
			outputFile << "\n**** You selected 'Medium Sensitivty'. ****" << endl;
		}
		else if (response == '4') {
			Sensitivity = 8000;
			cout << "\n**** You selected 'Low Sensitivty'. ****" << endl;
			outputFile << "\n**** You selected 'Low Sensitivty'. ****" << endl;
		}
		else if (response == '5') {
			Sensitivity = 15000; //12500
			cout << "\n**** You selected 'Very-Low Sensitivty'. ****" << endl;
			outputFile << "\n**** You selected 'Very-Low Sensitivty'. ****" << endl;
		}
		else {
			cout << "You didn't press a valid key." << endl;
			cout << "Begin the program, again." << endl;
			Sleep(1000);
			break;
		}

		acquire_image(backgroundcolor1, cam_number);    //Remove here and next line, later
		view_rgb_image(backgroundcolor1);
		cout << "\n\nProcess Beginning: Background/Normal is being initialized." << endl;
		Sleep(300);
		scale(backgroundcolor1, backgroundcolor2);
		copy(backgroundcolor2, backgroundgrey1);

		copy(backgroundgrey1, backgroundgrey2);
		threshold(backgroundgrey2, backgroundgrey1, 150);
		invert(backgroundgrey1, backgroundgrey2);

		timecount = 0; //Initialize counting time, so it begins from 0;

		cout << "\nBackground/Normal has been initialized." << endl;
		Sleep(1000);
		
		while (1)
		{
			if (KEY('X')) exit(0);
			timecount += 4000;
			timeSeconds = timecount / 1000;
			image_compare;
			acquire_image(color1, cam_number);

			copy(color1, grey1);

			threshold(grey1, grey2, 150);
			invert(grey2, grey1);

			subtract_image(backgroundgrey2, grey1, image_compare);
			image_pixelcount(image_compare, pixelcount1);

			if ((pixelcount1 != 0) && (pixelcount1 > Sensitivity))
			{
				cout << "\a**** At time: " << timeSeconds << " seconds, motion was detected! ****\n\n" << endl;
				outputFile << "**** At time: " << timeSeconds << " seconds, motion was detected! ****\n\n" << endl;
				Sleep(4000);
			}
			else 
			{
				cout << "At time: " << timeSeconds << " seconds, there was no motion detected!\n\n" << endl;
				outputFile << "At time: " << timeSeconds << " seconds, there was no motion detected!\n\n" << endl;
				Sleep(4000);
			}

			copy(grey1, backgroundgrey2);
		}		
	}

	free_image(color1);
	free_image(color2);
	free_image(grey1);
	free_image(grey2);
	free_image(backgroundcolor1);
	free_image(backgroundcolor2);
	free_image(backgroundgrey1);
	free_image(backgroundgrey2);
	free_image(image_compare);

	deactivate_cameras();
	outputFile.close();
	return 0;
}





