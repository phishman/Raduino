#ifndef RADUINO_H
#define RADUINO_H

#define VER	1.0

/*
/* Retro-compatibility with arduino 0023 and previous version */
#if ARDUINO >= 100
#include "Arduino.h"
#define I2CWRITE(x) Wire.write(x)
#define I2CREAD() Wire.read()
#else
#include "WProgram.h"
#define I2CWRITE(x) Wire.send(x)
#define I2CREAD() Wire.receive()
#endif
*/

#include <inttypes.h>


#define LED_OFF 0x0
#define LED_RED 0x1
#define LED_LIME 0x2
#define LED_YELLOW 0x3
#define LED_BLUE 0x4
#define LED_MAGENTA 0x5
#define LED_CYAN 0x6
#define LED_WHITE 0x7


class RADUINO {
public:
  RADUINO();
  void begin(void);

protected:

private:

};


#endif