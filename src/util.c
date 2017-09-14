#include "util.h"
#include "main.h"

void tankDriveControl(bool enabled, int rev, int divbyforlow) {
  if (joystickGetDigital(1, 7, JOY_RIGHT)){
    divbyforlow = 4;
  }

  int joy2 = joystickGetAnalog(1, 2) / divbyforlow;
  int joy3 = joystickGetAnalog(1, 3) / divbyforlow;
  
  if (enabled) {
    motorSet (2, joy3);
    motorSet (3, rev * joy3);
    motorSet (4, -1 * joy2);
    motorSet (5, rev * -1 * joy2);
  }
}

void liftControl(bool enabled) {
  int joy4 = joystickGetAnalog(1, 4);
  if (enabled){
    motorSet(6, joy4);
    motorSet(7, joy4);
    motorSet(8, joy4);
    motorSet(9, joy4);
  }
}
