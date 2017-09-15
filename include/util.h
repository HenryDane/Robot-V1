#ifndef _util_h
#define _util_h
#include <stdbool.h>

// Declare drive function
// enabled determines whether the drive operates
// rev determines whether any channels are reversed
// divbyforlow is the factor to divide the power by for low speed
void tankDriveControl(bool enabled, int rev, int divbyforlow);

// Declare lift function
void liftControl(bool enabled);

#endif
