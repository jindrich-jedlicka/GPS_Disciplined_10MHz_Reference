#ifndef _MENU_RUNTIME_MODULE
#define _MENU_RUNTIME_MODULE

#include "ubx_cfg_gnss.h"
#include "RuntimeModule.h"
#include "RuntimeContext.h"
#include "UBXGPS.h"

#define GNSS_IDS_LEN 2
#define MAX_MENU_TIME_MS 10000

typedef enum MENU_ITEM : uint8_t
{
  MENU_ITEM_GPS = 0,
  MENU_ITEM_GPS_SBAS = 1,
  MENU_ITEM_GAL = 2,
  MENU_ITEM_GAL_SBAS = 3,
  MENU_ITEM_GLO = 4,
  MENU_ITEM_GLO_SBAS = 5,
  MENU_ITEM_DIAG = 6,
  MENU_ITEM_ESC = 7,

  MENU_ITEM_LAST,
} MENU_ITEM;

typedef enum MENU_STATE : uint8_t
{
  MENU_STATE_NEW = 0,
  MENU_STATE_GNSS,
  MENU_STATE_CFG,
  MENU_STATE_DONE,
} MENU_STATE;

static GNSS_ID gnss_ids[] = { GNSS_ID_GPS, GNSS_ID_SBAS, GNSS_ID_GALILEO, GNSS_ID_BEI_DOU, GNSS_ID_QZSS, GNSS_ID_GLONASS };

class MenuRuntimeModule : public RuntimeModule
{
public:
  MenuRuntimeModule() : RuntimeModule(MENU_ITEM_LAST - 1)
  {
    _step_start_time = 0;
    _state = MENU_STATE_NEW;
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_MENU; };

protected:
  virtual void on_init()
  {
    _ubx_gps.init(&RuntimeContext::get_gps_stream());

    GpsLiquidCrystal& dsp = RuntimeContext::get_display();

    init_dsp("GNSS Options:");

    _state = MENU_STATE_NEW;
    _step_start_time = millis();
    set_new_pos(0);
  }

  virtual void on_loop()
  {
    if (_state != MENU_STATE_NEW)
    {
      if (STEP_TIME_MS <= (millis() - _step_start_time))
      {
        switch (_state)
        {
          case MENU_STATE_GNSS:
            print_details("GNSS");
            UBXGPS::print_result(send_gnss_cfg());
            set_state(MENU_STATE_CFG);
            break;

          case MENU_STATE_CFG:
            print_details("CFG");
            UBXGPS::print_result(_ubx_gps.save_all_to_bbr());
            set_state(MENU_STATE_DONE);
            break;

          case MENU_STATE_DONE:
            _ubx_gps.hw_reset();
            set_next_module(MODULE_TYPE_GPS_MONITOR);      
            break;
        }
      }
    }
    else if (MAX_MENU_TIME_MS <= (millis() - _step_start_time))
    {
      set_next_module(MODULE_TYPE_GPS_MONITOR);      
    }
  }

  virtual void on_encoder_pressed()
  {
    switch (get_encoder_pos())
    {
      case MENU_ITEM_GPS:
        _gnss_ids[0] = GNSS_ID_GPS;
        _gnss_ids[1] = GNSS_ID_GPS;
        break;

      case MENU_ITEM_GPS_SBAS:
        _gnss_ids[0] = GNSS_ID_GPS;
        _gnss_ids[1] = GNSS_ID_SBAS;
        break;

      case MENU_ITEM_GAL:
        _gnss_ids[0] = GNSS_ID_GALILEO;
        _gnss_ids[1] = GNSS_ID_GALILEO;
        break;

      case MENU_ITEM_GAL_SBAS:
        _gnss_ids[0] = GNSS_ID_GALILEO;
        _gnss_ids[1] = GNSS_ID_SBAS;
        break;

      case MENU_ITEM_GLO:
        _gnss_ids[0] = GNSS_ID_GLONASS;
        _gnss_ids[1] = GNSS_ID_GLONASS;
        break;

      case MENU_ITEM_GLO_SBAS:
        _gnss_ids[0] = GNSS_ID_GLONASS;
        _gnss_ids[1] = GNSS_ID_SBAS;
        break;

      case MENU_ITEM_DIAG:
        set_next_module(MODULE_TYPE_DIAG);
        return;

      default:
        set_next_module(MODULE_TYPE_GPS_MONITOR);
        return;
    }

    set_state(MENU_STATE_GNSS);
  }

  virtual void on_encoder_moved(uint8_t pos)
  {
    if (_state == MENU_STATE_NEW)
    {
      print_menu(pos);
      _step_start_time = millis();
    }
  }

private:
  void set_state(MENU_STATE state)
  {
    _step_start_time = millis();
    _state = state;
  }

  void print_menu(uint8_t pos)
  {
    switch (pos)
    {
      case MENU_ITEM_GPS:
        print_details("GPS");
        break;

      case MENU_ITEM_GPS_SBAS:
        print_details("GPS & SBAS");
        break;

      case MENU_ITEM_GAL:
        print_details("GALILEO");
        break;

      case MENU_ITEM_GAL_SBAS:
        print_details("GALILEO & SBAS");
        break;

      case MENU_ITEM_GLO:
        print_details("GLONASS");
        break;

      case MENU_ITEM_GLO_SBAS:
        print_details("GLONASS & SBAS");
        break;

      case MENU_ITEM_DIAG:
        print_details("USB Diagnostic");
        break;

      case MENU_ITEM_ESC:
        print_details("Back to monitor");
        break;
    }
  }

  static bool contains_id(const GNSS_ID id, const uint8_t ids_len, const GNSS_ID* ids)
  {
    for (uint8_t i = 0; i < ids_len; i++)
    {
      if (ids[i] == id)
        return true;
    }
    return false;
  }

  ACK_RESULT send_gnss_cfg()
  {
    const uint8_t len = sizeof(gnss_ids) / sizeof(gnss_ids[0]);
    ubx_cfg_gnss_block_t gnss[len];

    for (uint8_t i = 0; i < len; i++)
    {
      gnss[i] = ubx_cfg_gnss_block_t(gnss_ids[i], contains_id(gnss_ids[i], GNSS_IDS_LEN, _gnss_ids));
    }

    ubx_cfg_gnss_t gnss_base;
    gnss_base.num_config_blocks = len;

    return _ubx_gps.send_msg(msg_id_t(CAT_CFG, CFG_GNSS), sizeof(gnss_base), (uint8_t *)&gnss_base, sizeof(gnss), (uint8_t *)&gnss);
  }

private:
  GNSS_ID _gnss_ids[GNSS_IDS_LEN];
  UBXGPS _ubx_gps;
  unsigned long _step_start_time;
  MENU_STATE _state;
};

#endif // _MENU_RUNTIME_MODULE