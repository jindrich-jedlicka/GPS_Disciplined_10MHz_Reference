#include <NMEAGPS.h>
#include <NeoSWSerial.h>
#include <RotaryEncoder.h>
#include "GpsLiquidCrystal.h"
#include "GpsView.h"

static GpsLiquidCrystal lcd;

#define PIN_IN1 A3
#define PIN_IN2 A2
RotaryEncoder *encoder;

#define PIN_RX 2
#define PIN_TX 3
static NeoSWSerial gpsSerial(PIN_RX, PIN_TX);

static NMEAGPS nmeaGps;
static int pos = 0;

static GpsView gpsView;

void setup()
{
  lcd.begin();
  //defining pins if they are INPUT or OUTPUT pins
  //  pinMode(start_pin, INPUT);
  //  pinMode(stop1_pin, INPUT);
  //  pinMode(reset_pin, INPUT);
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);

  lcd.setCursor(0, 0);
  lcd.print("Neo-8M GPS DO ");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for data");
  encoder->tick(); // just call tick() to check the state.

  lcd.setCursor(0, 1);
  //lcd.print(pos, HEX);

  gpsSerial.begin(9600);
  Serial.begin(9600);
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

    // lcd.setCursor(0, 1);
    // lcd.print("        ");
    // lcd.setCursor(0, 1);
    //lcd.print(pos, HEX);
  }

  while (nmeaGps.available(gpsSerial)) 
  {
    gpsView.display_data(lcd, nmeaGps);
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
