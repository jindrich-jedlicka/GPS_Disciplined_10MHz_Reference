#ifndef _CONFIG_RUNTIME_MODULE
#define _CONFIG_RUNTIME_MODULE

#include "RuntimeModule.h"
#include "RuntimeContext.h"
#include "ubx_cfg_nav5.h"
#include "ubx_cfg_tp5.h"
#include "UBXGPS.h"

class ConfigRuntimeModule : public RuntimeModule
{
public:
  ConfigRuntimeModule() : RuntimeModule(0)
  {
    // do nothing
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_CONFIG; }

protected:
  virtual void on_init()
  {
    //TODO: use gps
    //_gps_stream = &RuntimeContext::get_gps_stream();
    _ubx_gps.init(&Serial);
  }

  virtual void on_loop()
  {
    send_tp5_cfg();
    set_next_module(MODULE_TYPE_GPS_MONITOR);
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
  void send_tp5_cfg()
  {
    ubx_cfg_tp5_t tp5_data;
    tp5_data.tp_idx = TP_IDX_TIMEPULSE;
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

    _ubx_gps.send_ubx_msg(msg_id_t(CAT_CFG, CFG_TP5), sizeof(tp5_data), (uint8_t *)&tp5_data);
  }

  void send_nav_cfg()
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

    _ubx_gps.send_ubx_msg(msg_id_t(CAT_CFG, CFG_NAV5), sizeof(nav_data), (uint8_t *)&nav_data);
  }

/*
  checksum_t calculate_checksum(checksum_t& csum, const uint8_t *data, const uint16_t len) const
  {
    for (uint16_t i = 0; i < len; i++)
    {
      csum.ck_a += data[i];
      csum.ck_b += csum.ck_a;
    }
    return csum;
  }
*/

/*
  bool get_UBX_ack(uint8_t *MSG) 
  {
    uint8_t b;
    uint8_t ackByteID = 0;
    uint8_t ackPacket[10];
    unsigned long startTime = millis();

    // Construct the expected ACK packet   
    ackPacket[0] = 0xB5;  // header
    ackPacket[1] = 0x62;  // header
    ackPacket[2] = 0x05;  // class
    ackPacket[3] = 0x01;  // id
    ackPacket[4] = 0x02;  // length
    ackPacket[5] = 0x00;
    ackPacket[6] = MSG[2];  // ACK class
    ackPacket[7] = MSG[3];  // ACK id
    ackPacket[8] = 0;   // CK_A
    ackPacket[9] = 0;   // CK_B

   // Calculate the checksums

  for (uint8_t i=2; i<8; i++) {

    ackPacket[8] = ackPacket[8] + ackPacket[i];

    ackPacket[9] = ackPacket[9] + ackPacket[8];

  }

   while (1) {

     // Test for success

    if (ackByteID > 9) {

      // All packets in order!

      Serial.println(" (SUCCESS!)");

      return true;

    }

     // Timeout if no valid response in 3 seconds

    if (millis()-startTime > 3000) {

      Serial.println(" (FAILED!) Try Again");

      return false;

    }

     // Make sure data is available to read

    if (GPS.available()) {

      b = GPS.read();

       // Check that bytes arrive in sequence as per expected ACK packet

      if (b == ackPacket[ackByteID]) {

        ackByteID++;

        PrnHEX(b);

      }

      else {

        ackByteID = 0;  // Reset and look again, invalid order

      }

     }

  }
}  
*/
private:
  UBXGPS _ubx_gps;
};

#endif