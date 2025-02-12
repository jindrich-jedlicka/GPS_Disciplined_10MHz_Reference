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
    _dsp.begin();
    _gpsSerial.begin(9600);
    Serial.begin(9600);

    _encoder_pos = 0;
    _encoder.setPosition(_encoder_pos);
    _encoder.tick(); 
  }

  static int get_encoder_delta()
  {
    int delta = 0;
    int newPos = _encoder.getPosition();
    if (newPos != _encoder_pos)
    {    
      delta = newPos - _encoder_pos;
      _encoder_pos = newPos;    
    }  
    return delta;
  }

  static inline void encoder_tick() { _encoder.tick(); }
  static inline GpsLiquidCrystal& get_display() { return _dsp; }
  static inline Stream& get_gps_stream() { return _gpsSerial; }
  static inline Stream& get_pc_stream() { return Serial; }

private:
  static GpsLiquidCrystal _dsp;
  static int _encoder_pos;
  static RotaryEncoder _encoder;
  static NeoSWSerial _gpsSerial;
};

static GpsLiquidCrystal RuntimeContext::_dsp;
static RotaryEncoder RuntimeContext::_encoder(_PIN_IN1, _PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
static int RuntimeContext::_encoder_pos = 0;
static NeoSWSerial RuntimeContext::_gpsSerial(_PIN_RX, _PIN_TX);

#endif