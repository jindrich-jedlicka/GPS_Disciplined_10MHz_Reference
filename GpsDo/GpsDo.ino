#include <NMEAGPS.h>
#include <NeoSWSerial.h>
#include <RotaryEncoder.h>
#include "GpsLiquidCrystal.h"
#include "GpsView.h"
#include "SateliteInfoView.h"
#include "SateliteData.h"
#include "GpsMonitor.h"
#include "RuntimeContext.h"
#include "ModuleManager.h"

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

static ModuleManager modules;
/*
static GpsLiquidCrystal lcd;

#define PIN_IN1 A3
#define PIN_IN2 A2
static int pos = 0;
static RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

#define PIN_RX 2
#define PIN_TX 3
static NeoSWSerial gpsSerial(PIN_RX, PIN_TX);

static GpsMonitor gpsMonitor;
*/
/*
static NMEAGPS nmeaGps;
static int index = 0;

static GpsView gpsView;
static SateliteInfoView satView;
static View *activeView = NULL;
static SateliteData gpsData;
*/
void setup()
{
  //lcd.begin();
  //defining pins if they are INPUT or OUTPUT pins
  //  pinMode(start_pin, INPUT);
  //  pinMode(stop1_pin, INPUT);
  //  pinMode(reset_pin, INPUT);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);
/*
  lcd.setCursor(0, 0);
  lcd.print("Neo-8M GPS DO ");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for data");
  encoder.tick(); 

  gpsSerial.begin(9600);
  Serial.begin(9600);

  gpsMonitor.begin();
*/
  //////////////////////////////
  RuntimeContext::setup();
  modules.setup();
}

ISR(PCINT0_vect)
{
  NeoSWSerial::rxISR(PINB);
}

ISR(PCINT1_vect)
{
  NeoSWSerial::rxISR(PINC);
  RuntimeContext::encoder_tick();
  //encoder.tick(); // just call tick() to check the state.
}

ISR(PCINT2_vect)
{
  NeoSWSerial::rxISR(PIND);
}
/*
char readChar;

bool data_available()
{
    update_index();
    while (gpsSerial.available())
    {
      nmeaGps.handle( gpsSerial.read() );
      update_index();
    }
    return nmeaGps.available();
}
*/
void loop()
{
  modules.loop();
/*
  while (gpsSerial.available())
  {
    gpsMonitor.char_received( gpsSerial.read() );
    update_index();
  }
  gpsMonitor.data_transfer_completed(lcd);
  while (data_available()) 
  {
    gpsData.set_data(nmeaGps);
    if (activeView != NULL)
      activeView->display_data(lcd, index, gpsData);
  }  
  update_index();*/
//  if (gpsSerial.available())
//  {
//    readChar = gpsSerial.read();
//    Serial.write(readChar);
//    //gpsParser.encode(readChar);
//  }
  /*
  if (Serial.available() > 0)
  {
    readChar = Serial.read(); 
//    lcd.setCursor(5, 1);
//    lcd.print(readChar, HEX);
    gpsSerial.write(readChar);
  }
  */
}

/*void set_active_view(View *view)
{
  activeView = view;

  if (activeView != NULL)
    activeView->clear();
}

void update_index()
{
  int newPos = encoder.getPosition();
  if (newPos != pos)
  {    
    gpsMonitor.encoder_moved(lcd, newPos - pos);
  /*  int prevIndex = index;
    index += newPos - pos;
    if (index < 0)
      index = 0;
    else if (MAX_INDEX < index)
      index = MAX_INDEX;

    if (prevIndex != index)
    {
      if (index == 0)
        set_active_view(&gpsView);
      else if (prevIndex == 0)
        set_active_view(&satView);

      if (gpsData.is_data_set())
        activeView->display_data(lcd, index, gpsData);
    }
    pos = newPos;    
  }  
 }*/