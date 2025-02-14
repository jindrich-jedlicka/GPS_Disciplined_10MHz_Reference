#ifndef _RUNTIME_MODULE
#define _RUNTIME_MODULE

typedef enum MODULE_TYPE
{
  MODULE_TYPE_MENU,
  MODULE_TYPE_UPLOAD_CONFIG,
  MODULE_TYPE_GPS_MONITOR,
  MODULE_TYPE_GPS_DIAG,
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
    on_init();
  }
  virtual MODULE_TYPE loop() = 0;

protected:
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
    set_new_index((int8_t)tmp);
  }

  inline uint8_t get_encoder_pos() const { return _pos; }
  virtual void on_init() = 0;
  virtual void on_encoder_pressed() = 0;
  virtual void on_encoder_moved(uint8_t pos) = 0;

  void set_new_index(int8_t new_pos)
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
};

#endif