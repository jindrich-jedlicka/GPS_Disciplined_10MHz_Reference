#ifndef _RUNTIME_CONTEXT
#define _RUNTIME_CONTEXT

#include <RotaryEncoder.h>
#include <NeoSWSerial.h>
#include "GpsLiquidCrystal.h"

#define _PIN_IN1 A3
#define _PIN_IN2 A2
#define _PIN_BTN A1

#define _PIN_RX 2
#define _PIN_TX 3

#define _DEBOUNCE_DELAY 50UL

class EncoderButton
{
public:
  EncoderButton(int pin)
  {
    _pin = pin;
    _lastState = true;
    _state = true;
    _lastDebounceTime = 0;
  }

public:
  void setup()
  {
    pinMode(_pin, INPUT_PULLUP);

    _lastState = true;
    _state = true;
    _lastDebounceTime = millis();
  }

  bool test_state_change() 
  {
    bool stateModified = false;
    bool reading = digitalRead(_pin);  // Read button state
    unsigned long millisNow = millis();

    if (reading != _lastState) 
    {
        _lastDebounceTime = millisNow;
    }

    if (_DEBOUNCE_DELAY < (millisNow - _lastDebounceTime)) 
    {
        if (reading != _state) 
        {
            _state = reading;
            stateModified = true;
        }
    }

    _lastState = reading;    
    return stateModified;
  }

  inline bool is_pressed() const { return !_state; }

private:
  unsigned long _lastDebounceTime;
  int _pin;
  bool _lastState;
  bool _state;
};

class RuntimeContext
{
public:
  static void setup()
  {
    _dsp.begin();
    _gpsSerial.begin(9600);
    Serial.begin(9600);

    _encoder_pos = 0;
    _encoder_button.setup();
    _encoder_button_pressed = _encoder_button.is_pressed();
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

  static bool encoder_button_pressed()
  {
    if (_encoder_button.test_state_change() && _encoder_button.is_pressed() != _encoder_button_pressed)
    {
      _encoder_button_pressed = !_encoder_button_pressed;
      return _encoder_button_pressed;
    }
    return false;
  }

  static inline void encoder_tick() { _encoder.tick(); }
  static inline GpsLiquidCrystal& get_display() { return _dsp; }
  static inline Stream& get_gps_stream() { return _gpsSerial; }
  static inline Stream& get_pc_stream() { return Serial; }

private:
  static GpsLiquidCrystal _dsp;
  static int _encoder_pos;
  static bool _encoder_button_pressed;
  static EncoderButton _encoder_button;
  static RotaryEncoder _encoder;
  static NeoSWSerial _gpsSerial;
};

static GpsLiquidCrystal RuntimeContext::_dsp;
static EncoderButton RuntimeContext::_encoder_button(_PIN_BTN);
static bool RuntimeContext::_encoder_button_pressed = false;
static RotaryEncoder RuntimeContext::_encoder(_PIN_IN1, _PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
static int RuntimeContext::_encoder_pos = 0;
static NeoSWSerial RuntimeContext::_gpsSerial(_PIN_RX, _PIN_TX);

#endif