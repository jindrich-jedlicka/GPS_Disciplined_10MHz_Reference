#ifndef _RUNTIME_CONTEXT
#define _RUNTIME_CONTEXT

#include <RotaryEncoder.h>
#include <NeoSWSerial.h>
#include "GpsLiquidCrystal.h"

#define _PIN_IN1 A3
#define _PIN_IN2 A2

#define _PIN_RX 2
#define _PIN_TX 3

class RuntimeContext
{
public:
  static void setup()
  {
    dsp.begin();
    gpsSerial.begin(9600);
    Serial.begin(9600);
    encoder.tick(); 
  }

  static int get_encoder_delta()
  {
    int delta = 0;
    int newPos = encoder.getPosition();
    if (newPos != encoder_pos)
    {    
      delta = newPos - encoder_pos;
      encoder_pos = newPos;    
    }  
    return delta;
  }

  static inline void encoder_tick() { encoder.tick(); }
  static inline GpsLiquidCrystal& get_display() { return dsp; }
  static inline Stream& get_gps_stream() { return gpsSerial; }
  static inline Stream& get_pc_stream() { return Serial; }

private:
  static GpsLiquidCrystal dsp;
  static int encoder_pos;
  static RotaryEncoder encoder;
  static NeoSWSerial gpsSerial;
};

static RotaryEncoder RuntimeContext::encoder(_PIN_IN1, _PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
static int RuntimeContext::encoder_pos = 0;
static NeoSWSerial RuntimeContext::gpsSerial(_PIN_RX, _PIN_TX);

#endif