#include "util.h"
#include "main.h"

void tankDriveControl(bool enabled, int rev, int divbyforlow) {
  // if 7R then divide power by 4
  if (joystickGetDigital(1, 7, JOY_RIGHT)){
    divbyforlow = 4;
  }

  // get the values from the joystick
  int joy2 = joystickGetAnalog(1, 2) / divbyforlow;
  int joy3 = joystickGetAnalog(1, 3) / divbyforlow;

  // if enabled then write to the motors
  if (enabled) {
    motorSet (2, joy3);
    motorSet (3, rev * joy3);
    motorSet (4, -1 * joy2);
    motorSet (5, rev * -1 * joy2);
  }
}

void liftControl(bool enabled) {
  // power variable
  int joy4 = 0;

  // if 8U then set joy4 to full power down
  if (joystickGetDigital(1, 8, JOY_UP)){
    joy4 = 127;
  }

  // if 8D then set joy4 to full power down
  if (joystickGetDigital(1, 8, JOY_DOWN)){
    joy4 = -127;
  }

  // if enabled then write joy4 to motors 8 and 9
  if (enabled){
    motorSet(8, joy4);
    motorSet(9, - joy4);
  }
}

void secondaryLift(void){
  // power variable
  int power = 0;

  // if 8R then full power up
  if (joystickGetDigital(1, 8, JOY_RIGHT)){
    power = 127;
  }

  // if 8L then full power down
  if(joystickGetDigital(1, 8, JOY_LEFT)){
    power = -127;
  }

  // write to channel 7 - secondary channel
  motorSet(7, power);
}

void handleClaw(void){
  // power variable
  int power = 0;

  // if 6D then full power up
  if (joystickGetDigital(1, 6, JOY_DOWN)){
    power = 127;
  }

  // if 6U then full power down
  if(joystickGetDigital(1, 6, JOY_UP)){
    power = -127;
  }

  // write to channel 6 - secondary channel
  motorSet(6, power);
}
