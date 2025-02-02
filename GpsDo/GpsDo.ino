#include <NMEAGPS.h>
#include <NeoSWSerial.h>
#include <RotaryEncoder.h>
#include <LiquidCrystal.h>

//setting up LCD INPUT pins
#define PIN_RS 12
#define PIN_ENABLE 11
#define PIN_D4 10
#define PIN_D5 9
#define PIN_D6 8
#define PIN_D7 7

LiquidCrystal lcd(PIN_RS, PIN_ENABLE, PIN_D4, PIN_D5, PIN_D6, PIN_D7);

#define PIN_IN1 A3
#define PIN_IN2 A2
RotaryEncoder *encoder;

#define PIN_RX 2
#define PIN_TX 3
NeoSWSerial gpsSerial(PIN_RX, PIN_TX);

static NMEAGPS nmeaGps;
static int pos = 0;

#define INVALID_VAL 0xFF

//////////////////////////////////////////////////////////////////////////////
// UTC Time
//////////////////////////////////////////////////////////////////////////////

#define TIME_LINE 0
#define TIME_START_POS 6
#define TIME_HOUR_POS TIME_START_POS + 2
#define TIME_MINUTE_POS TIME_HOUR_POS + 3
#define TIME_SEC_POS TIME_MINUTE_POS + 3

static uint8_t _sec;
static uint8_t _minute;
static uint8_t _hour;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Satelites
//////////////////////////////////////////////////////////////////////////////

#define SAT_LINE 0
#define SAT_START_POS 0

static uint8_t _sat;

//////////////////////////////////////////////////////////////////////////////

void setup()
{
  lcd.begin(16 , 2); //starting LCD

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
  lcd.print(pos, HEX);

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
    lcd.print(pos, HEX);
  }

  while (nmeaGps.available(gpsSerial)) 
  {
    gps_fix fix = nmeaGps.read();
    if (nmeaGps.satellites_valid())
    {      
      lcd.setCursor(8, 1);
      lcd.print(nmeaGps.sat_count);

      int tracked = 0;
      for (uint8_t i = 0; i < nmeaGps.sat_count; i++)
      {
        if (nmeaGps.satellites[i].tracked)
          tracked++;
      }
      lcd.setCursor(5, 1);
      lcd.print(tracked);      

      display_sat(fix.satellites);
    }
    if (fix.valid.time) 
    {
      display_hour(fix.dateTime.hours);
      display_minute(fix.dateTime.minutes);
      display_second(fix.dateTime.seconds);
    }
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

void init_sat()
{
  _sat = INVALID_VAL;
  
  lcd.setCursor(SAT_START_POS, SAT_LINE);    
  lcd.print("S:  ");
}

void display_sat(uint8_t value)
{
  if (_sat != value)
  {
    display_number(value, SAT_LINE, SAT_START_POS + 2);
    _sat = value;
  }
}

void init_time()
{
  _sec = INVALID_VAL;
  _minute = INVALID_VAL;
  _hour = INVALID_VAL;  

  lcd.setCursor(TIME_START_POS, TIME_LINE);    
  lcd.print("T:  :  :  ");
}

void display_hour(uint8_t value)
{
  if (_hour != value)
  {
    display_number(value, TIME_LINE, TIME_HOUR_POS);    
    _hour = value;
  }
}

void display_minute(uint8_t value)
{
  if (_minute != value)
  {
    display_number(value, TIME_LINE, TIME_MINUTE_POS);    
    _minute = value;
  }
}

void display_second(uint8_t value)
{
  if (_sec != value)
  {
    display_number(value, TIME_LINE, TIME_SEC_POS);
    _sec = value;
  }
}

void display_number(uint8_t number, uint8_t line, uint8_t pos)
{
  lcd.setCursor(pos, line);      
  if (number < 10)
    lcd.print("0");
    
  lcd.print(number);
}
