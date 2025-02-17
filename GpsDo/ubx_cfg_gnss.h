#ifndef _UBX_CFG_GNSS
#define _UBX_CFG_GNSS

typedef enum GNSS_ID : uint8_t
{
  GNSS_ID_GPS = 0,
  GNSS_ID_SBAS = 1,
  GNSS_ID_GALILEO = 2,
  GNSS_ID_BEI_DOU = 3,
  GNSS_ID_IMES = 4,
  GNSS_ID_QZSS = 5,
  GNSS_ID_GLONASS = 6,
} GNSS_ID;

typedef enum GNSS_FLAG : uint32_t
{
  GNSS_FLAG_ENABLE = 0x01,

  GNSS_FLAG_GPS_L1C_A = 0x010000,
  GNSS_FLAG_GPS_L2C = 0x100000,
  GNSS_FLAG_GPS_L5 = 0x200000,

  GNSS_FLAG_SBAS_L1C_A = 0x010000,

  GNSS_FLAG_GALILEO_E1 = 0x010000,
  GNSS_FLAG_GALILEO_E5A = 0x100000,
  GNSS_FLAG_GALILEO_E5B = 0x200000,

  GNSS_FLAG_BEI_DOU_B1I = 0x010000,
  GNSS_FLAG_BEI_DOU_B2I = 0x100000,
  GNSS_FLAG_BEI_DOU_B2A = 0x800000,

  GNSS_FLAG_QZSS_L1C_A = 0x010000,
  GNSS_FLAG_QZSS_L1S = 0x040000,
  GNSS_FLAG_QZSS_L2C = 0x100000,
  GNSS_FLAG_QZSS_L5 = 0x200000,

  GNSS_FLAG_GLONASS_L1 = 0x010000,
  GNSS_FLAG_GLONASS_L2 = 0x100000,
} GNSS_FLAG;

typedef struct ubx_cfg_gnss_block_t
{
  GNSS_ID   gnss_id;            // GNSS identifier (see Satellite Numbering)
  uint8_t   res_trk_ch;         // Number of reserved (minimum) tracking channels for this GNSS system
  uint8_t   max_trk_ch;         // Maximum number of tracking channels used for this GNSS system (>= res_trk_ch)
  uint8_t   reserved1;          // Reserved
  GNSS_FLAG flags;              // Bitfield of flags (see graphic in datasheet)

  ubx_cfg_gnss_block_t(GNSS_ID _id, bool enable = true)
  {
    gnss_id = _id;
    reserved1 = 0x00;

    switch (_id)
    {
      case GNSS_ID_GPS:
        res_trk_ch = 8;
        max_trk_ch = 13;
        flags = GNSS_FLAG_GPS_L1C_A;
        break;

      case GNSS_ID_SBAS:
        res_trk_ch = 3;
        max_trk_ch = 3;
        flags = GNSS_FLAG_SBAS_L1C_A;
        break;

      case GNSS_ID_GALILEO:
        res_trk_ch = 8;
        max_trk_ch = 12;
        flags = GNSS_FLAG_GALILEO_E1;
        break;

      case GNSS_ID_BEI_DOU:
        res_trk_ch = 2;
        max_trk_ch = 5;
        flags = GNSS_FLAG_BEI_DOU_B1I;
        break;

      case GNSS_ID_IMES:
        res_trk_ch = 0;
        max_trk_ch = 0;
        flags = 0;
        break;

      case GNSS_ID_QZSS:
        res_trk_ch = 3;
        max_trk_ch = 4;
        flags = GNSS_FLAG_QZSS_L1C_A | GNSS_FLAG_QZSS_L1S;
        break;

      case GNSS_ID_GLONASS:
        res_trk_ch = 8;
        max_trk_ch = 12;
        flags = GNSS_FLAG_GLONASS_L1;
        break;
    }
    if (enable)
      flags |= GNSS_FLAG_ENABLE;
  }
} ubx_cfg_gnss_block_t;

typedef struct ubx_cfg_gnss_t
{
  uint8_t   msg_ver;            // Message version (=0 for this version)
  uint8_t   num_trk_ch_hw;      // Number of tracking channels available in hardware (read only)
  uint8_t   num_trk_ch_use;     // Number of tracking channels to use (<= num_trk_ch_hw)
  uint8_t   num_config_blocks;  // Number of configuration blocks following

  ubx_cfg_gnss_t()
  {
    msg_ver = 0x00;
    num_trk_ch_hw = 0x00;
    num_trk_ch_use = 0x0FF;
    num_config_blocks = 0;
  }
} ubx_cfg_gnss_t;

#endif // _UBX_CFG_GNSS