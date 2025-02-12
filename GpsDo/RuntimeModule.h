#ifndef _RUNTIME_MODULE
#define _RUNTIME_MODULE

typedef enum MODULE_TYPE
{
  MODULE_TYPE_NONE,
  MODULE_TYPE_MENU,
  MODULE_TYPE_UPLOAD_CONFIG,
  MODULE_TYPE_GPS_MONITOR,
  MODULE_TYPE_GPS_DIAG,
} MODULE_TYPE;

class RuntimeModule
{
public:
  RuntimeModule()
  {
  }

public:
  virtual void begin() = 0;
  virtual MODULE_TYPE loop() = 0;
};

#endif