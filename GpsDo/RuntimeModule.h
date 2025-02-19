#ifndef _RUNTIME_MODULE
#define _RUNTIME_MODULE

#define STEP_TIME_MS 1000 

typedef enum MODULE_TYPE : uint8_t
{
  MODULE_TYPE_MENU = 0,
  MODULE_TYPE_CONFIG,
  MODULE_TYPE_GPS_MONITOR,
  MODULE_TYPE_DIAG,
} MODULE_TYPE;

class RuntimeModule
{
public:
  RuntimeModule(uint8_t max_pos)
  {
    _max_pos = max_pos;
    _pos = -1;
  }

public:
  virtual MODULE_TYPE get_type() = 0;
  void begin()
  {
    _pos = -1;
    _loop_return_val = get_type();
    on_init();
  }
  MODULE_TYPE loop()
  {
    check_encoder();
    on_loop();
    return _loop_return_val;
  }

protected:
  static void init_dsp(const String &text)
  {
    GpsLiquidCrystal& dsp = RuntimeContext::get_display();

    dsp.clear();
    dsp.setCursor(0, 0);
    dsp.print(text);
  }

  inline void print_details(const String &text)
  {
    RuntimeContext::get_display().print_at_row(1, text);
  }

  void check_encoder()
  {
    if (RuntimeContext::encoder_button_pressed())
    {
      RuntimeContext::get_encoder_delta(); // reset encoder movement
      on_encoder_pressed();
    }
    else
    {
      int delta = RuntimeContext::get_encoder_delta();
      if (0 != delta)
      {
        encoder_moved(delta);
      }
    }
  }

  void encoder_moved(int delta)
  {
    int tmp = (int)_pos + delta;
    if (tmp < 0)
    {
      tmp = 0;
    }
    else if (_max_pos < tmp)
    {
      tmp = _max_pos;
    }    
    set_new_pos((int8_t)tmp);
  }

  inline void set_next_module(MODULE_TYPE loop_return_val) { _loop_return_val = loop_return_val; }
  inline uint8_t get_encoder_pos() const { return _pos; }
  virtual void on_init() = 0;
  virtual void on_loop() = 0;
  virtual void on_encoder_pressed() = 0;
  virtual void on_encoder_moved(uint8_t pos) = 0;

  void set_new_pos(int8_t new_pos)
  {
    if (_pos != new_pos)
    {
      on_encoder_moved(new_pos);
      _pos = new_pos;
    }
  }

private:
  uint8_t _max_pos;
  uint8_t _pos;
  MODULE_TYPE _loop_return_val;
};

#endif