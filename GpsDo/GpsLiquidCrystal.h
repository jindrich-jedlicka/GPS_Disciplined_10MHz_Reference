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
  const uint8_t MAX_BAR_VALUE = 7;

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
    clear();
  }

public:
  uint8_t print_bar_val(uint8_t val)
  {
    if (MAX_BAR_VALUE < val)
      val = MAX_BAR_VALUE;

    return write(val);
  }

  uint8_t print_n_char(const uint8_t len, const char char_to_fill)
  {
    for (uint8_t i = 0; i < len; i++) {
      print(char_to_fill);
    }
    return len;
  }

  uint8_t print_dec_number(uint16_t n, const uint8_t len, const char prefix_char = '\0')
  {
    if (len == 0)
      return 0;
    
    char buf[len]; 
    char *str = &buf[sizeof(buf)];
    uint8_t i = 0;

    do {
      char c = n % 10;
      n /= 10;

      *--str = c + '0';
      i++;
    } while(n && i < len);

    if (prefix_char != '\0') {
      while (i < len) {
        *--str = prefix_char;
        i++;
      }
    }
    uint8_t bytes_written = write(str, (size_t)i);
    while (bytes_written < len) {
      write(' ');
      bytes_written++;
    }
    return bytes_written;
  }
};

#endif  // _GPS_LIQUID_CRYSTAL