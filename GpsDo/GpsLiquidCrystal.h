#ifndef _GPS_LIQUID_CRYSTAL
#define _GPS_LIQUID_CRYSTAL

#include <LiquidCrystal.h>

//setting up LCD INPUT pins
#define PIN_RS 12
#define PIN_ENABLE 11
#define PIN_D4 10
#define PIN_D5 9
#define PIN_D6 8
#define PIN_D7 7

#define DISP_ROWS 2
#define DISP_COLS 16

byte one_line[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte two_lines[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
};

byte three_lines[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
};

byte four_lines[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte five_lines[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte six_lines[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte seven_lines[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte eight_lines[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

class GpsLiquidCrystal : public LiquidCrystal {
public:
  GpsLiquidCrystal()
    : LiquidCrystal(PIN_RS, PIN_ENABLE, PIN_D4, PIN_D5, PIN_D6, PIN_D7) 
  {
    // do nothing
  }

public:
  void begin() 
  {
    createChar(0, one_line);
    createChar(1, two_lines);
    createChar(2, three_lines);
    createChar(3, four_lines);
    createChar(4, five_lines);
    createChar(5, six_lines);
    createChar(6, seven_lines);
    createChar(7, eight_lines);
    LiquidCrystal::begin(DISP_COLS, DISP_ROWS);  //starting LCD
  }
};

#endif  // _GPS_LIQUID_CRYSTAL