#ifndef _VIEW
#define _VIEW

#include "SateliteData.h"
#include "GpsLiquidCrystal.h"

class View
{
public:
  View()
  {
    _initialized = false;
  }

public:
  void display_data(GpsLiquidCrystal& dsp, uint8_t index, const SateliteData& gpsData)
  {
    if (!_initialized)
    {
      dsp.clear();
      on_init(dsp);
      
      _initialized = true;
    }
    on_display_data(dsp, index, gpsData);
  }

  void clear()
  {
    if (_initialized)
    {
      on_clear();
      _initialized = false;
    }
  }

protected:
  virtual void on_init(GpsLiquidCrystal& dsp) = 0;
  virtual void on_display_data(GpsLiquidCrystal& dsp, uint8_t index, const SateliteData& gpsData) = 0;
  virtual void on_clear() = 0;

private:
  bool _initialized;
};

#endif