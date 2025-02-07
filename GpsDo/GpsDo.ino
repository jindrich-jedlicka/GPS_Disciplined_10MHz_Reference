#include <NMEAGPS.h>
#include <NeoSWSerial.h>
#include <RotaryEncoder.h>
#include "GpsLiquidCrystal.h"
#include "GpsView.h"
#include "SateliteInfoView.h"

//--------------------
// Check configuration

#ifndef NMEAGPS_PARSE_GSV
  #error You must define NMEAGPS_PARSE_GSV in NMEAGPS_cfg.h!
#endif

#ifndef NMEAGPS_PARSE_SATELLITES
  #error You must define NMEAGPS_PARSE_SATELLITE in NMEAGPS_cfg.h!
#endif

#ifndef NMEAGPS_PARSE_SATELLITE_INFO
  #error You must define NMEAGPS_PARSE_SATELLITE_INFO in NMEAGPS_cfg.h!
#endif

//--------------------

#define MAX_INDEX NMEAGPS_MAX_SATELLITES + 1

static GpsLiquidCrystal lcd;

#define PIN_IN1 A3
#define PIN_IN2 A2
static RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

#define PIN_RX 2
#define PIN_TX 3
static NeoSWSerial gpsSerial(PIN_RX, PIN_TX);

static NMEAGPS nmeaGps;
static int pos = 0;
static int index = 0;

static GpsView gpsView;
static SateliteInfoView satView;

void setup()
{
  lcd.begin();
  //defining pins if they are INPUT or OUTPUT pins
  //  pinMode(start_pin, INPUT);
  //  pinMode(stop1_pin, INPUT);
  //  pinMode(reset_pin, INPUT);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);

  lcd.setCursor(0, 0);
  lcd.print("Neo-8M GPS DO ");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for data");
  encoder.tick(); 

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
  encoder.tick(); // just call tick() to check the state.
}

ISR(PCINT2_vect)
{
  NeoSWSerial::rxISR(PIND);
}

char readChar;

void loop()
{
  update_index();
  while (nmeaGps.available(gpsSerial)) 
  {
    if (index == 0)
    {
      gpsView.display_data(lcd, nmeaGps);
    }
    else
    {
      const NMEAGPS::satellite_view_t *p_satelite_data = NULL;
      uint8_t sat_index = (uint8_t)index - 1;

      if (sat_index < nmeaGps.sat_count)
        p_satelite_data = &nmeaGps.satellites[sat_index];

      satView.display_data(lcd, index, p_satelite_data);
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

void update_index()
{
  int newPos = encoder.getPosition();
  if (newPos != pos)
  {    
    int prevIndex = index;
    index += newPos - pos;
    if (index < 0)
      index = 0;
    else if (MAX_INDEX < index)
      index = MAX_INDEX;

    if (prevIndex != index)
    {
      if (index == 0)
        gpsView.clear();
      else if (prevIndex == 0)      
        satView.clear();
    }
    pos = newPos;    
  }  
 }