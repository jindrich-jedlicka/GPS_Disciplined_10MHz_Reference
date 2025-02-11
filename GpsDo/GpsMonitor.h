#ifndef _GPS_MONITOR
#define _GPS_MONITOR

#include <NMEAGPS.h>
#include "GpsView.h"
#include "SateliteInfoView.h"

#define MAX_INDEX NMEAGPS_MAX_SATELLITES

class GpsMonitor
{
public:
  GpsMonitor()
  {
    _activeView = NULL;
    _index = 0;
  }

public:
  void begin()
  {
    _satView.clear();
    _mainView.clear();
    _gpsData.clear();

    _index = 0;
    _activeView = &_mainView;
    _nmeaGps.reset();
  }

  inline void char_received( uint8_t c ) { _nmeaGps.handle(c); }

  void data_transfer_completed(GpsLiquidCrystal& dsp)
  {
    if (_nmeaGps.available())
    {
      _gpsData.set_data(_nmeaGps);

      if (_activeView != NULL)
        _activeView->display_data(dsp, _index, _gpsData);
    }
  }

  void button_pushed(GpsLiquidCrystal& dsp)
  {
    if (_index != 0)
    {
      set_new_index(dsp, 0);
      _index = 0;
    }
  }

  void encoder_moved(GpsLiquidCrystal& dsp, int8_t delta)
  {
    int16_t tmp = _index + delta;
    if (tmp < 0)
    {
      tmp = 0;
    }
    else if (MAX_INDEX < tmp)
    {
      tmp = MAX_INDEX;
    }
    if (_index != (int8_t)tmp)
    {
      set_new_index(dsp, (int8_t)tmp);
    }
  }
  
private:
  void set_new_index(GpsLiquidCrystal& dsp, int8_t newIndex)
  {
      if (_index == 0)
        set_active_view(&_satView);
      else if (newIndex == 0)
        set_active_view(&_mainView);

      _index = newIndex;

      if (_gpsData.is_data_set())
        _activeView->display_data(dsp, _index, _gpsData);
  }

  void set_active_view(View *view)
  {
    if (_activeView != view)
    {
      _activeView = view;

      if (_activeView != NULL)
      {
        _activeView->clear();
        _nmeaGps.reset();
      }
    }
  }

private:
  NMEAGPS _nmeaGps;
  SateliteData _gpsData;
  View *_activeView;
  GpsView _mainView;
  SateliteInfoView _satView;
  int8_t _index;
};

#endif // _GPS_MONITOR