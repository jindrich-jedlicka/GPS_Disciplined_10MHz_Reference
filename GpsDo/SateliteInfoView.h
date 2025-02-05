#ifndef _SATELITE_INFO_VIEW
#define _SATELITE_INFO_VIEW

#include <NMEAGPS.h>
#include "GpsLiquidCrystal.h"



class SateliteInfoView
{
public:
  SateliteInfoView()
  {
    _initialized = false;
  }

public:
  void display_data(GpsLiquidCrystal& lcd, const satellite_view_t& satelite_data)
  {

  }

  void clear()
  {
    _initialized = false;
  }

private:
  satellite_view_t _satelite_data;
  bool _initialized;
};

#endif // _SATELITE_INFO_VIEW