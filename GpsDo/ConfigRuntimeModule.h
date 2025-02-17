#ifndef _CONFIG_RUNTIME_MODULE
#define _CONFIG_RUNTIME_MODULE

#include "RuntimeModule.h"
#include "RuntimeContext.h"
#include "ubx_cfg_nav5.h"
#include "ubx_cfg_tp5.h"
#include "ubx_cfg_gnss.h"
#include "UBXGPS.h"

#define STEP_TIME_MS 1000 

typedef enum CONFIG_STEP : uint8_t
{
  CONFIG_STEP_TP5,
  CONFIG_STEP_NAV5,
  CONFIG_STEP_WAITING_DATA,
  CONFIG_STEP_DONE,
} CONFIG_STEP;

class ConfigRuntimeModule : public RuntimeModule
{
public:
  ConfigRuntimeModule() : RuntimeModule(0)
  {
    _step_start_time = 0;
    _step = CONFIG_STEP_TP5;
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_CONFIG; }

protected:
  virtual void on_init()
  {
    _ubx_gps.init(&RuntimeContext::get_gps_stream());

    _step_start_time = millis() - STEP_TIME_MS;
    _step = CONFIG_STEP_TP5;
    init_dsp();
  }

  virtual void on_loop()
  {
    if (STEP_TIME_MS <= (millis() - _step_start_time))
    {
      switch (_step)
      {
        case CONFIG_STEP_TP5:
          print_step("TP5");
          print_result(send_tp5_cfg());
          set_step(CONFIG_STEP_NAV5);
          break;

        case CONFIG_STEP_NAV5:
          print_step("NAV");
          print_result(send_nav_cfg());
          set_step(CONFIG_STEP_WAITING_DATA);
          break;

        case CONFIG_STEP_WAITING_DATA:
          print_step("Waiting for data");
          set_step(CONFIG_STEP_DONE);
          break;

        case CONFIG_STEP_DONE:
          set_next_module(MODULE_TYPE_GPS_MONITOR);
          break;
      }
    }
  }

  virtual void on_encoder_pressed() 
  {
    // nothing to do
  }

  virtual void on_encoder_moved(uint8_t pos) 
  {
    // should not happen
  }

private:
  void set_step(CONFIG_STEP step)
  {
    _step_start_time = millis();
    _step = step;
  }

  void init_dsp()
  {
    GpsLiquidCrystal& dsp = RuntimeContext::get_display();

    dsp.clear();
    dsp.setCursor(0, 0);
    dsp.print("Neo-8M GPS:");
  }

  void print_result(ACK_RESULT result)
  {
    switch (result)
    {
      case ACK_RESULT_TIMEOUT:
        RuntimeContext::get_display().print(" TIMEOUT");
        break;
      case ACK_RESULT_FALSE:
        RuntimeContext::get_display().print(" FAILED");
        break;
      case ACK_RESULT_TRUE:
        RuntimeContext::get_display().print(" OK");
        break;
    }
  }

  void print_step(const String &text)
  {
    GpsLiquidCrystal& dsp = RuntimeContext::get_display();
    dsp.setCursor(0, 1);
    dsp.print("                ");
    dsp.setCursor(0, 1);
    dsp.print(text);    
  }

  ACK_RESULT send_tp5_cfg()
  {
    ubx_cfg_tp5_t tp5_data;
    tp5_data.tp_idx = TP_IDX_TIMEPULSE2;
    tp5_data.version = 0x01;
    tp5_data.ant_cable_delay = 50; // [ns]
    tp5_data.rf_group_delay = 0;
    tp5_data.freq_per = 1; // Hz
    tp5_data.freq_per_lock = 100000; // Hz
    tp5_data.pulse_len_ratio = 0x80000000;
    tp5_data.pulse_len_ratio_lock = 0x80000000;
    tp5_data.user_config_delay = 0;
    tp5_data.flags = CFG_TP5_V1_ACTIVE
      | CFG_TP5_V1_LOCK_GNSS_FREQ 
      | CFG_TP5_V1_LOCKED_OTHER_SET
      | CFG_TP5_V1_IS_FREQ
      | CFG_TP5_V1_ALGN_TO_TOW
      | CFG_TP5_V1_POLARITY;

    return _ubx_gps.send_msg(msg_id_t(CAT_CFG, CFG_TP5), sizeof(tp5_data), (uint8_t *)&tp5_data);
  }

  ACK_RESULT send_nav_cfg()
  {
    ubx_cfg_nav5_t nav_data;

    nav_data.mask = CFG_NAV5_MASK_ALL;
    nav_data.dyn_model = DYN_MODEL_STATIONARY;
    nav_data.fix_mode = FIX_MODE_AUTO;
    nav_data.fixed_alt = 0;
    nav_data.fixed_alt_var = 10000; // scaling 0.0001, [m^2] Fixed altitude variance for 2D mode 
    nav_data.min_elv = 5; // [deg], Minimum elevation for a GNSS satellite to be used in NAV 
    nav_data.p_dop = 250; // scaling 0.1, Position DOP mask to use
    nav_data.t_dop = 250; // scaling 0.1, Time DOP mask to use
    nav_data.p_acc = 100; // [m] Position accuracy mask
    nav_data.t_acc = 300; // [m] Time accuracy mask
    nav_data.stat_hold_trsh = 0; // [cm/s] Static hold threshold
    nav_data.dgnss_timeout = 60; // [s] DGNSS timeout
    nav_data.cno_trsh_num_SVs = 0; // Number of satellites required to have C/N0 above cnoThresh for a fix to be attempted
    nav_data.cno_trsh = 0; // [dBHz] C/N0 threshold for deciding whether to attempt a fix
    nav_data.stat_hold_max_distance = 0; // [m] Static hold distance threshold (before quitting static hold)
    nav_data.utc_standard = UTC_STANDARD_AUTO;

    return _ubx_gps.send_msg(msg_id_t(CAT_CFG, CFG_NAV5), sizeof(nav_data), (uint8_t *)&nav_data);
  }

private:
  UBXGPS _ubx_gps;
  unsigned long _step_start_time;
  CONFIG_STEP _step;
};

#endif