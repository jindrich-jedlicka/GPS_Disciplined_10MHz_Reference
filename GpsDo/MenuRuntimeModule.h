#ifndef _MENU_RUNTIME_MODULE
#define _MENU_RUNTIME_MODULE

#include "RuntimeModule.h"

class MenuRuntimeModule : public RuntimeModule
{
public:
  MenuRuntimeModule() : RuntimeModule(5)
  {

  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_MENU; };

protected:
  virtual void on_init()
  {

  }

  virtual void on_loop()
  {
    
  }

  virtual void on_encoder_pressed()
  {

  }

  virtual void on_encoder_moved(uint8_t pos)
  {

  }


private:

};

#endif // _MENU_RUNTIME_MODULE