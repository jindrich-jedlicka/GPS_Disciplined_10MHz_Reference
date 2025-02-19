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

void loop()
{
  modules.loop();
}
