#include <NMEAGPS.h>
#include <NeoSWSerial.h>
#include <RotaryEncoder.h>
#include <LiquidCrystal.h>

#define INVALID_VAL 0xFF

//////////////////////////////////////////////////////////////////////////////
// Display
//////////////////////////////////////////////////////////////////////////////

//setting up LCD INPUT pins
#define PIN_RS 12
#define PIN_ENABLE 11
#define PIN_D4 10
#define PIN_D5 9
#define PIN_D6 8
#define PIN_D7 7

#define DISP_ROWS 2
#define DISP_COLS 16

LiquidCrystal lcd(PIN_RS, PIN_ENABLE, PIN_D4, PIN_D5, PIN_D6, PIN_D7);

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

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Encoder
//////////////////////////////////////////////////////////////////////////////

#define PIN_IN1 A3
#define PIN_IN2 A2
RotaryEncoder *encoder;

#define PIN_RX 2
#define PIN_TX 3
NeoSWSerial gpsSerial(PIN_RX, PIN_TX);

static NMEAGPS nmeaGps;
static int pos = 0;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// UTC Time
//////////////////////////////////////////////////////////////////////////////

#define TIME_ROW 0
#define TIME_COL 8
#define TIME_PREFIX_LEN 0 // "T:"
#define TIME_BLOCK_LEN 3  // "00:"
#define TIME_HOUR_COL TIME_COL + TIME_PREFIX_LEN
#define TIME_MINUTE_COL TIME_HOUR_COL + TIME_BLOCK_LEN
#define TIME_SEC_COL TIME_MINUTE_COL + TIME_BLOCK_LEN

static uint8_t _sec;
static uint8_t _minute;
static uint8_t _hour;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Satelites
//////////////////////////////////////////////////////////////////////////////

#define SAT_ROW 0
#define SAT_COL 0
#define SAT_PREFIX_LEN 4 // "SAT:"

static uint8_t _sat;
static uint8_t _sat_info_count;

//////////////////////////////////////////////////////////////////////////////

void setup()
{
  lcd.createChar(0, one_line);
  lcd.createChar(1, two_lines);
  lcd.createChar(2, three_lines);
  lcd.createChar(3, four_lines);
  lcd.createChar(4, five_lines);
  lcd.createChar(5, six_lines);
  lcd.createChar(6, seven_lines);
  lcd.createChar(7, eight_lines);
  lcd.begin(DISP_COLS, DISP_ROWS); //starting LCD

  //defining pins if they are INPUT or OUTPUT pins
  //  pinMode(start_pin, INPUT);
  //  pinMode(stop1_pin, INPUT);
  //  pinMode(reset_pin, INPUT);
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);

  lcd.setCursor(0, 0);
  //lcd.print("Rotary Encoder");
  encoder->tick(); // just call tick() to check the state.

  lcd.setCursor(0, 1);
  //lcd.print(pos, HEX);

  gpsSerial.begin(9600);
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Neo-8M GPSDO ");

  init_sat();
  init_time();
}

ISR(PCINT0_vect)
{
  NeoSWSerial::rxISR(PINB);
}

ISR(PCINT1_vect)
{
  NeoSWSerial::rxISR(PINC);
  encoder->tick(); // just call tick() to check the state.
}

ISR(PCINT2_vect)
{
  NeoSWSerial::rxISR(PIND);
}

char readChar;

void loop()
{
  //encoder->tick(); // just call tick() to check the state.

  int newPos = encoder->getPosition();
  if (newPos != pos)
  {
    pos = newPos;

    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(0, 1);
    //lcd.print(pos, HEX);
  }

  while (nmeaGps.available(gpsSerial)) 
  {
    const gps_fix& fix = nmeaGps.read();
    if (nmeaGps.satellites_valid())
    {      
      display_satelites_info();
//      lcd.setCursor(8, 1);
//      lcd.print(nmeaGps.sat_count);
//
//      int tracked = 0;
//      for (uint8_t i = 0; i < nmeaGps.sat_count; i++)
//      {
//        if (nmeaGps.satellites[i].tracked)
//          tracked++;
//      }
//      lcd.setCursor(5, 1);
//      lcd.print(tracked);      
      
    }
    display_sat(fix.satellites);
    display_time(fix);
  }  
//  if (gpsSerial.available())
//  {
//    readChar = gpsSerial.read();
//    Serial.write(readChar);
//    //gpsParser.encode(readChar);
//  }
  
  if (Serial.available() > 0)
  {
    readChar = Serial.read(); 
//    lcd.setCursor(5, 1);
//    lcd.print(readChar, HEX);
    gpsSerial.write(readChar);
  }
}

void checkPosition()
{
  encoder->tick(); // just call tick() to check the state.
}

//////////////////////////////////////////////////////////////////////////////
void display_satelites_info()
{
  int count = nmeaGps.sat_count;
  if (DISP_COLS < count)
    count = DISP_COLS;

  lcd.setCursor(0, 1);
  for (uint8_t i = 0; i < count; i++)
  {
    if (nmeaGps.satellites[i].tracked)
    {
      lcd.write(byte(toBarNumber(nmeaGps.satellites[i].snr)));
    }
    else
    {
      lcd.print('x');
    }
  }
  if (count < _sat_info_count)
  {
    for (uint8_t i = count; i < _sat_info_count; i++)
    {
      lcd.print(' ');
    }
  }
  _sat_info_count = count;
}

#define SCALE_DIV 10000
#define SCALE_DIV_HALF SCALE_DIV >> 1
#define MAX_BAR_VALUE 7

uint8_t toBarNumber(uint8_t snr)
{
  uint32_t val = snr;
  val *= 1000;
  val = val / SCALE_DIV;
  
  if (SCALE_DIV_HALF < (val % SCALE_DIV))
    val++;

   if (MAX_BAR_VALUE < val)
    val = MAX_BAR_VALUE;
   
   return (uint8_t)val;
}

//////////////////////////////////////////////////////////////////////////////
void init_sat()
{
  _sat = INVALID_VAL;
  _sat_info_count = 0;
  
  lcd.setCursor(SAT_COL, SAT_ROW);    
  lcd.print("SAT ? ");
}

void display_sat(uint8_t value)
{
  if (_sat != value)
  {
    lcd.setCursor(SAT_COL + SAT_PREFIX_LEN, SAT_ROW);      
    
    lcd.print(value);
    if (value < 10)
      lcd.print(' ');
      
    _sat = value;
  }
}

//////////////////////////////////////////////////////////////////////////////
void init_time()
{
  _sec = INVALID_VAL;
  _minute = INVALID_VAL;
  _hour = INVALID_VAL;  
  
  display_unknown_time();
}

void display_unknown_time()
{
  lcd.setCursor(TIME_COL, TIME_ROW);    
  lcd.print("??:??:??");
}

void display_time(const gps_fix& fix)
{
    if (fix.valid.time) 
    {
      display_hour(fix.dateTime.hours);
      display_minute(fix.dateTime.minutes);
      display_second(fix.dateTime.seconds);
    }
    else
    {
      display_unknown_time();    
    }
}

void display_hour(uint8_t value)
{
  if (_hour != value)
  {
    display_number(value, TIME_ROW, TIME_HOUR_COL);    
    _hour = value;
  }
}

void display_minute(uint8_t value)
{
  if (_minute != value)
  {
    display_number(value, TIME_ROW, TIME_MINUTE_COL);    
    _minute = value;
  }
}

void display_second(uint8_t value)
{
  if (_sec != value)
  {
    display_number(value, TIME_ROW, TIME_SEC_COL);
    _sec = value;
  }
}

void display_number(uint8_t number, uint8_t row, uint8_t col)
{
  lcd.setCursor(col, row);      
  if (number < 10)
    lcd.print("0");
    
  lcd.print(number);
}
