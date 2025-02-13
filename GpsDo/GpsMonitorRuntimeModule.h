#ifndef _GPS_MONITOR_RUNTIME_MODULE
#define _GPS_MONITOR_RUNTIME_MODULE

#include <NMEAGPS.h>
#include "SatellitesView.h"
#include "SateliteInfoView.h"
#include "RuntimeModule.h"
#include "RuntimeContext.h"

#define MAX_INDEX NMEAGPS_MAX_SATELLITES

class GpsMonitorRuntimeModule : public RuntimeModule
{
public:
  GpsMonitorRuntimeModule()
  {
    init();
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_GPS_MONITOR; }

  virtual void begin()
  {
    init();
    set_new_index(0);
  }

  virtual MODULE_TYPE loop()
  {
    while (RuntimeContext::get_gps_stream().available())
    {
      _nmeaGps.handle(RuntimeContext::get_gps_stream().read());
      check_encoder();
    }
    data_transfer_completed();
    check_encoder();

    return MODULE_TYPE_GPS_MONITOR;
  }

private:
  void init()
  {
    _activeView = NULL;
    _index = -1;

    _satView.clear();
    _mainView.clear();
    _gpsData.clear();
    _nmeaGps.reset();
  }

  void check_encoder()
  {
    int delta = RuntimeContext::get_encoder_delta();
    if (0 != delta)
      encoder_moved(delta);
  }

  void data_transfer_completed()
  {
    if (_nmeaGps.available())
    {
      _gpsData.set_data(_nmeaGps);

      if (_activeView != NULL)
        _activeView->display_data(RuntimeContext::get_display(), _index, _gpsData);
    }
  }

  void button_pushed()
  {
    set_new_index(0);
  }

  void encoder_moved(int delta)
  {
    int tmp = _index + delta;
    if (tmp < 0)
    {
      tmp = 0;
    }
    else if (MAX_INDEX < tmp)
    {
      tmp = MAX_INDEX;
    }
    set_new_index((int8_t)tmp);
  }
  
  void set_new_index(int8_t newIndex)
  {
    if (_index != newIndex)
    {
      if (_index == 0)
        set_active_view(&_satView);
      else if (newIndex == 0)
        set_active_view(&_mainView);

      _index = newIndex;

      if (_gpsData.is_data_set())
        _activeView->display_data(RuntimeContext::get_display(), _index, _gpsData);
    }
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
  SatellitesView _mainView;
  SateliteInfoView _satView;
  View *_activeView;
  int8_t _index;
};

#endif // _GPS_MONITOR_RUNTIME_MODULE