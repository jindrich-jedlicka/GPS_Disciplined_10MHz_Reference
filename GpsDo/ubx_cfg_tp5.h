#ifndef _UBX_CFG_TP5
#define _UBX_CFG_TP5

typedef enum TP_IDX : uint8_t
{
  TP_IDX_TIMEPULSE = 0,
  TP_IDX_TIMEPULSE2 = 1,
} TP_IDX;

typedef enum CFG_TP5_V0 : uint32_t
{
  CFG_TP5_V0_ACTIVE = 0x0001,
  CFG_TP5_V0_LOCK_GPS_FREQ = 0x0002,
  CFG_TP5_V0_LOCKED_OTHER_SET = 0x0004,  
  CFG_TP5_V0_IS_FREQ = 0x0008,
  CFG_TP5_V0_IS_LENGTH = 0x0010,
  CFG_TP5_V0_ALGN_TO_TOW = 0x0020,
  CFG_TP5_V0_POLARITY = 0x0040,
  CFG_TP5_V0_GRID_UTC_GPS = 0x0080,
} CFG_TP5_V0;

typedef enum CFG_TP5_V1 : uint32_t
{
  CFG_TP5_V1_ACTIVE = 0x0001,
  CFG_TP5_V1_LOCK_GNSS_FREQ = 0x0002,  
  CFG_TP5_V1_LOCKED_OTHER_SET = 0x0004,
  CFG_TP5_V1_IS_FREQ = 0x0008,
  CFG_TP5_V1_IS_LENGTH = 0x0010,
  CFG_TP5_V1_ALGN_TO_TOW = 0x0020,
  CFG_TP5_V1_POLARITY = 0x0040,
  CFG_TP5_V1_GRD_UTC_GNSS = 0x0800,
  CFG_TP5_V1_SYNC_MODE = 0x4000,
} UBX_CFG_TP5_FLAGS;

typedef struct ubx_cfg_tp5_t
{
  ubx_cfg_tp5_t()
  {
    tp_idx = 0;
    version = 0;
    reserved_1_1 = 0;
    reserved_1_2 = 0;
    ant_cable_delay = 0;
    rf_group_delay = 0;
    freq_per = 0;
    freq_per_lock = 0;
    pulse_len_ratio = 0;
    pulse_len_ratio_lock = 0;
    user_config_delay = 0;
    flags = 0;
  }

  TP_IDX        tp_idx;
  uint8_t       version;
  uint8_t       reserved_1_1;
  uint8_t       reserved_1_2;
  int16_t       ant_cable_delay; // [ns] Antenna cable delay
  int16_t       rf_group_delay; // [ns] RF group delay
  uint32_t      freq_per; // [Hz or mikro-sec] Frequency or period time, depending on setting of bit 'CFG_TP5_Vx_IS_FREQ'
  uint32_t      freq_per_lock;  // [Hz or mikro-sec] Frequency or period time when locked to GPS time, only used if 'CFG_TP5_Vx_LOCKED_OTHER_SET' is set
  uint32_t      pulse_len_ratio; // [mikro-sec or 2^32] Pulse length or duty cycle, depending on CFG_TP5_Vx_IS_LENGTH 
  uint32_t      pulse_len_ratio_lock; // [mikro-sec or 2^32] Pulse length or duty cycle, depending on CFG_TP5_Vx_IS_LENGTH 
  int32_t       user_config_delay; // [ns] User-configurable time pulse delay
  uint32_t      flags; // see CFG_TP5_V1
} ubx_cfg_tp5_t;

#endif //_UBX_CFG_TP5