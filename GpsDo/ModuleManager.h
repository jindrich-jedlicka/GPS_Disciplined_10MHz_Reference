#ifndef _MODULE_MANAGER
#define _MODULE_MANAGER

#include "RuntimeModule.h"
#include "GpsMonitorRuntimeModule.h"

class ModuleManager
{
public:
  ModuleManager()
  {
    _pActiveModule = NULL;
    _moduleType = MODULE_TYPE_NONE;
  }

public:
  void setup()
  {
    set_module(MODULE_TYPE_GPS_MONITOR);
  }

  void loop()
  {
    if (_pActiveModule != NULL)
    {
      set_module(_pActiveModule->loop());
    }
  }

private:
  void set_module(MODULE_TYPE moduleType)
  {
      if (_moduleType != moduleType)
      {
        _pActiveModule = get_module(moduleType);
        if (_pActiveModule != NULL)
        {
          _pActiveModule->begin();
        }
        _moduleType = moduleType;
      }

  }
  RuntimeModule* get_module(MODULE_TYPE moduleType)
  {
    switch(moduleType)
    {
      case MODULE_TYPE_GPS_MONITOR:
        return &_gpsModule;

      // TODO:

      default:
        return NULL;
    }
  }

private:
  static GpsMonitorRuntimeModule _gpsModule;

  RuntimeModule* _pActiveModule;
  MODULE_TYPE _moduleType;
};

static GpsMonitorRuntimeModule ModuleManager::_gpsModule;

#endif // _MODULE_MANAGER