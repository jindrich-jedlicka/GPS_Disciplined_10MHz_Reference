#ifndef _RUNTIME_MODULE
#define _RUNTIME_MODULE

class RuntimeContext;

class RuntimeModule
{
public:
  RuntimeModule()
  {
    _pContext = NULL;
  }

public:
  void begin(RuntimeContext& context)
  {

  }

  void loop()
  {

  }

  void end()
  {
    _pContext = NULL;
  }

protected:
  virtual void on_begin(RuntimeContext& context) = 0;
  virtual void on_loop() = 0;
  virtual void on_end() = 0;

private:
  RuntimeContext* _pContext;
};

#endif