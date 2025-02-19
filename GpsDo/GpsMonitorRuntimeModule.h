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
  GpsMonitorRuntimeModule() : RuntimeModule(MAX_INDEX)
  {
    init();
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_GPS_MONITOR; }

protected:
  virtual void on_init()
  {
    init();
    set_new_pos(0);
  }

  virtual void on_loop()
  {
    if (RuntimeContext::get_gps_stream().available())
    {
      _nmeaGps.handle(RuntimeContext::get_gps_stream().read());
    }
    else if (_nmeaGps.available())
    {
      _gpsData.set_data(_nmeaGps);

      if (_activeView != NULL)
        _activeView->display_data(RuntimeContext::get_display(), get_encoder_pos(), _gpsData);
    }
  }

  virtual void on_encoder_pressed()
  {
    if (get_encoder_pos() != 0)
      set_new_pos(0);
    else
      set_next_module(MODULE_TYPE_MENU);
  }

  virtual void on_encoder_moved(uint8_t pos)
  {
      if (get_encoder_pos() == 0)
        set_active_view(&_satView);
      else if (pos == 0)
        set_active_view(&_mainView);

      if (_gpsData.is_data_set())
        _activeView->display_data(RuntimeContext::get_display(), pos, _gpsData);
  }

private:
  void init()
  {
    _activeView = NULL;

    _satView.clear();
    _mainView.clear();
    _gpsData.clear();
    _nmeaGps.reset();
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
};

#endif // _GPS_MONITOR_RUNTIME_MODULE