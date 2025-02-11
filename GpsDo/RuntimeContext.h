#ifndef _RUNTIME_CONTEXT
#define _RUNTIME_CONTEXT

#include <RotaryEncoder.h>
#include "RuntimeModule.h"

#define _PIN_IN1 A3
#define _PIN_IN2 A2

class RuntimeContext
{
public:
  RuntimeContext()
  {
    _activeModule = NULL;
    RotaryEncoder encoder(_PIN_IN1, _PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
  }

public:
  void setup()
  {

  }

  void loop()
  {

  }

  void set_active_module()
  {

  }

private:

  RuntimeModule* _activeModule;
  //RotaryEncoder encoder(_PIN_IN1, _PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
};

#endif