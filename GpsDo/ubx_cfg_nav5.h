#ifndef _UBX_CFG_NAV5
#define _UBX_CFG_NAV5

typedef enum CFG_NAV5_MASK : uint16_t
{
  CFG_NAV5_MASK_DYN = 0x0001, // Apply dynamic model settings
  CFG_NAV5_MASK_MIN_EL = 0x0002, // Apply minimum elevation settings
  CFG_NAV5_MASK_POS_FIX_MODE = 0x0004, // Apply fix mode settings
  CFG_NAV5_MASK_DR_LIM = 0x0008, // RESERVED
  CFG_NAV5_MASK_POS = 0x0010, // Apply position mask settings
  CFG_NAV5_MASK_TIME = 0x0020, // Apply time mask settings
  CFG_NAV5_MASK_STATIC_HOLD = 0x0040, // Apply static hold settings
  CFG_NAV5_MASK_DGPS = 0x0080, // Apply DGPS settings
  CFG_NAV5_MASK_CNO_TRSH = 0x0100, // Apply CNO threshold settings (cnoThresh, cnoThreshNumSVs)
  CFG_NAV5_MASK_UTC = 0x0400, // Apply UTC settings - not supported in protocol versions less than 16

  CFG_NAV5_MASK_ALL = CFG_NAV5_MASK_DYN 
    | CFG_NAV5_MASK_MIN_EL 
    | CFG_NAV5_MASK_POS_FIX_MODE 
    | CFG_NAV5_MASK_DR_LIM 
    | CFG_NAV5_MASK_POS 
    | CFG_NAV5_MASK_TIME
    | CFG_NAV5_MASK_STATIC_HOLD
    | CFG_NAV5_MASK_DGPS
    | CFG_NAV5_MASK_CNO_TRSH
    | CFG_NAV5_MASK_UTC,
} CFG_NAV5_MASK; 

typedef enum DYN_MODEL : uint8_t
{
  DYN_MODEL_PORTABLE = 0,
  DYN_MODEL_STATIONARY = 2,
  DYN_MODEL_PEDESTRIAN = 3,
  DYN_MODEL_AUTOMOTIVE = 4,
  DYN_MODEL_SEA = 5,
  DYN_MODEL_AIRBORNE_1G = 6,
  DYN_MODEL_AIRBORNE_2G = 7,
  DYN_MODEL_AIRBORNE_4G = 8,
  DYN_MODEL_WRIST_WORN_WATCH = 9, // not supported in protocol versions less than 18
  DYN_MODEL_MOTORBIKE = 10, // supported in protocol versions 19.2, and 35.10, and 35.15, and 35.20
  DYN_MODEL_ROBOTIC_LAWN = 11,
  DYN_MODEL_SCOOTER = 12,
} DYN_MODEL;

typedef enum FIX_MODE : uint8_t
{
  FIX_MODE_2D = 1,
  FIX_MODE_3D = 2,
  FIX_MODE_AUTO = 3,
} FIX_MODE;

typedef enum UTC_STANDARD : uint8_t
{
  UTC_STANDARD_AUTO = 0, // Automatic; receiver selects based on GNSS configuration
  UTC_STANDARD_USNO = 3,  // UTC as operated by the U.S. Naval Observatory (USNO); derived from GPS time
  UTC_STANDARD_GAL = 5, // UTC as combined from multiple European laboratories; derived from Galileo time
  UTC_STANDARD_SU = 6, // UTC as operated by the former Soviet Union (SU); derived from GLONASS time
  UTC_STANDARD_NTSC = 7, // UTC as operated by the National Time Service Center (NTSC), China; derived from BeiDou time
  UTC_STANDARD_NPLI = 8, // UTC as operated by the National Physics Laboratory, India (NPLI); derived from NavIC time
} UTC_STANDARD;

typedef struct ubx_cfg_nav5_t
{
  ubx_cfg_nav5_t()
  {
    mask = 0;
    dyn_model = DYN_MODEL_PORTABLE;
    fix_mode = FIX_MODE_AUTO;
    fixed_alt = 0;
    fixed_alt_var = 0;
    min_elv = 0;
    dr_limit = 0;
    p_dop = 0;
    t_dop = 0;
    p_acc = 0;
    t_acc = 0;
    stat_hold_trsh = 0;
    dgnss_timeout = 0;
    cno_trsh_num_SVs = 0;
    cno_trsh = 0;
    reserved_1_1 = 0;
    reserved_1_2 = 0;
    stat_hold_max_distance = 0;
    utc_standard = UTC_STANDARD_AUTO;
    reserved_2_1 = 0;
    reserved_2_2 = 0;
    reserved_2_3 = 0;
    reserved_2_4 = 0;
    reserved_2_5 = 0;
  }

  uint16_t      mask; // see CFG_NAV5_MASK
  DYN_MODEL     dyn_model;
  FIX_MODE      fix_mode;
  int32_t       fixed_alt; // scaling 0.01, [m] Fixed altitude (mean sea level) for 2D fix mode
  uint32_t      fixed_alt_var; // scaling 0.0001, [m^2] Fixed altitude variance for 2D mode
  int8_t        min_elv; // [deg], Minimum elevation for a GNSS satellite to be used in NAV 
  uint8_t       dr_limit; // [s] - RESERVED
  uint16_t      p_dop; // scaling 0.1, Position DOP mask to use
  uint16_t      t_dop; // scaling 0.1, Time DOP mask to use
  uint16_t      p_acc; // [m] Position accuracy mask
  uint16_t      t_acc; // [m] Time accuracy mask
  uint8_t       stat_hold_trsh; // [cm/s] Static hold threshold
  uint8_t       dgnss_timeout; // [s] DGNSS timeout
  uint8_t       cno_trsh_num_SVs; // Number of satellites required to have C/N0 above cnoThresh for a fix to be attempted
  uint8_t       cno_trsh; // [dBHz] C/N0 threshold for deciding whether to attempt a fix
  uint8_t       reserved_1_1;
  uint8_t       reserved_1_2;
  uint16_t      stat_hold_max_distance; // [m] Static hold distance threshold (before quitting static hold)
  UTC_STANDARD  utc_standard;
  uint8_t       reserved_2_1;
  uint8_t       reserved_2_2;
  uint8_t       reserved_2_3;
  uint8_t       reserved_2_4;
  uint8_t       reserved_2_5;
} ubx_cfg_nav5_t;

#endif