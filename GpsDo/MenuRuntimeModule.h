#ifndef _MENU_RUNTIME_MODULE
#define _MENU_RUNTIME_MODULE

#include "RuntimeModule.h"



class MenuRuntimeModule : public RuntimeModule
{
public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_MENU; };

  virtual void begin()
  {
    // TODO:
  } 

  virtual MODULE_TYPE loop()
  {
    // TODO:
    return MODULE_TYPE_MENU;
  }

private:

};

#endif // _MENU_RUNTIME_MODULE