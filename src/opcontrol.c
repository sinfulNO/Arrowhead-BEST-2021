/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *jkhjkhjkjk
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

int joystickGetAnalog ( unsigned char joystick,   // the joystick slot to check (1 for master, 2 for partner)
                         unsigned char axis        // One of the joystick channels on a VEX Joystick: 1, 2, 3, 4, ACCEL_X, or ACCEL_Y
                       );
void motorSet ( unsigned char channel,  // motor channel to set from 1-10
                int speed               // new signed speed. -127 is full reverse, 127 full forward, 0 off
              );

void chasisSet(double left, double right) {
  motorSet(2, left);
  motorSet(3, right);
}

void operatorControl() {
  int left;
  int right;
  int deadband = 10;
    while (1) {
        if(joystickGetAnalog(1,3) >= deadband || joystickGetAnalog(1,3) <= -deadband) {
          left = joystickGetAnalog(1, 3); // vertical axis on left joystick
        }
        else {
          left = 0;
        }
        if(joystickGetAnalog(1,2) >= deadband || joystickGetAnalog(1,2) <= -deadband) {
          right  = joystickGetAnalog(1, 2); // vertical axis on right joystick
        }
        else {
          right = 0;
        }
        chasisSet(left, right); // set chasis from joystck
        if(joystickGetDigital(1, 6, JOY_UP)) {  // code for left forklift
          motorSet(4, 127);
        }
        else if(joystickGetDigital(1, 6, JOY_DOWN)){
          motorSet(4, -127);
        }
        else {
          motorStop(4);
        }
        if(joystickGetDigital(1, 5, JOY_UP)) { // code for right forklift
          motorSet(5, -127);
        }
        else if(joystickGetDigital(1, 5, JOY_DOWN)){
          motorSet(5, -127);
        }
        else {
          motorStop(5);
        }
        if(joystickGetDigital(1, 7, JOY_UP) && joystickGetDigital(1, 8, JOY_UP)) { //autonomous code
          chasisSet(127, 127);
          delay(1000);
          chasisSet(0, 0);
        }
        delay(20);
    }
}
