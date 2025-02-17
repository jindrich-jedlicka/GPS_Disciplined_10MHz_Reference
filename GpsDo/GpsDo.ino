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

void setup()
{
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);

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
