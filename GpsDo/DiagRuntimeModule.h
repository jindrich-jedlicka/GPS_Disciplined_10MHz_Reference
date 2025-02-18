#ifndef _DIAG_RUNTIME_MODULE
#define _DIAG_RUNTIME_MODULE

#include "RuntimeContext.h"
#include "RuntimeModule.h"

class DiagRuntimeModule : public RuntimeModule
{
public:
  DiagRuntimeModule() : RuntimeModule(0)
  {
    // do nothing
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_DIAG; };

protected:
  virtual void on_init()
  {
    GpsLiquidCrystal& dsp = RuntimeContext::get_display();
    
    dsp.clear();
    dsp.setCursor(0, 0);
    dsp.print("GPS Diagnostic");
    dsp.setCursor(0, 1);
    dsp.print("USB 9600 Bd");
  }

  virtual void on_loop()
  {    
    uint8_t c;
    if (RuntimeContext::get_gps_stream().available())
    {
      c = RuntimeContext::get_gps_stream().read();
      Serial.write(c);
    }
    if (Serial.available() > 0)
    {
      c = Serial.read(); 
      RuntimeContext::get_gps_stream().write(c);
    }
  }

  virtual void on_encoder_pressed()
  {
    set_next_module(MODULE_TYPE_GPS_MONITOR);
  }

  virtual void on_encoder_moved(uint8_t pos)
  {
    // do nothing
  }
};

#endif // _DIAG_RUNTIME_MODULE