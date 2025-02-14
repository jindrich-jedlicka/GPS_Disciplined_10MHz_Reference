#ifndef _MODULE_MANAGER
#define _MODULE_MANAGER

#include "RuntimeModule.h"
#include "GpsMonitorRuntimeModule.h"
#include "MenuRuntimeModule.h"

class ModuleManager
{
public:
  ModuleManager()
  {
    _pActiveModule = NULL;
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
    if (_pActiveModule == NULL || _pActiveModule->get_type() != moduleType)
    {
      _pActiveModule = get_module(moduleType);
      if (_pActiveModule != NULL)
      {
        _pActiveModule->begin();
      }
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
  GpsMonitorRuntimeModule _gpsModule;
  MenuRuntimeModule _menuModule;

  RuntimeModule* _pActiveModule;
};

#endif // _MODULE_MANAGER