/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

// Include a utility header with function definitions which streamline the code
#include "util.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

// global variable
char str[10] = {};

// global display tool
void printCharToTopLine(int pos, char c){
	str[pos] = c;
	lcdSetText(uart2, 1, str);
}

void operatorControl() {
	// menu state
	int state = 0;

	// enable drive/lift
	bool drive = true;
	bool lift = true;

	// reversed front motors?
	int rev = 1;

	// display LCD
	lcdInit(uart2);
	lcdShutdown(uart2);
	lcdInit(uart2);

	// init char d array
	for (int i = 0; i < 10; i++){
		str[i] = ' ';
	}

	// let everyone know this somehow works
	lcdSetText(uart2, 2, "OPCONTROL ON");
	printCharToTopLine(0, '!');

	// voodo to setup the lcd
	delay(200);

	// wait for button 8U before doing anything
	// allow configuring
	while(! joystickGetDigital(1, 8, JOY_UP)) {

		// disable drive
		if (joystickGetDigital(1, 7, JOY_UP)){
			drive = false;
			printCharToTopLine(2,'D');
		}

		//disable lift
		if(joystickGetDigital(1, 7, JOY_DOWN)){
			lift = false;
			printCharToTopLine(3,'L');
		}

		if(joystickGetDigital(1, 7, JOY_LEFT)){
			rev = -1;
			printCharToTopLine(4,'R');
		}
	}

	// backlight on, update
	lcdSetBacklight(uart2, true);
	printCharToTopLine(0, '~');

	// main loop, motors 2 and 4 reversed
	while (1){
		// write to the drive motors
		tankDriveControl(drive, rev);

		// write to the lift motors
		liftControl(lift);

		// handle the menu
		if(joystickGetDigital(1, 7, JOY_UP)) { state++; }
		if(joystickGetDigital(1, 7, JOY_DOWN)) { state--; }

		// display the correct information on the lcd
		switch((state % 4)){
			case 0:
				lcdSetText(uart2, 2, "Device ON");
				break;
			case 1:
				lcdPrint(uart2, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
				break;
			case 2:
				lcdPrint(uart2, 2, "Back: %1.3f V", (double)powerLevelBackup() / 1000);
				break;
			case 3:
				lcdPrint(uart2, 2, "ACX: %d", joystickGetAnalog(1, ACCEL_X));
				break;
				lcdPrint(uart2, 2, "ACY:%d", joystickGetAnalog(1, ACCEL_Y));
				break;
			default:
				lcdSetText(uart2, 2, "Escaped sw-c!");
				break;
		}

		// wait to give the motors time to react
		delay(300);
	}
}
