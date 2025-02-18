#ifndef _UBX_CFG_RST
#define _UBX_CFG_RST

typedef enum NAV_BBR : uint16_t 
{
  NAV_BBR_HOT_START = 0x0000,
  NAV_BBR_WARM_START = 0x0001,
  NAV_BBR_COLD_START = 0xFFFF,

  NAV_BBR_EPH = 0x0001,     // Ephemeris
  NAV_BBR_ALM = 0x0002,     // Almanac
  NAV_BBR_HEALTH = 0x0004,  // Health
  NAV_BBR_KLOB = 0x0008,    // Klobuchar parameters
  NAV_BBR_POS = 0x0010,
  NAV_BBR_CLKD = 0x0020,    // Clock drift
  NAV_BBR_OSC = 0x0040,     // Oscillator parameter
  NAV_BBR_UTC = 0x0080,     // UTC correction + GPS leap seconds parameters
  NAV_BBR_RTC = 0x0100,     // RTC

  NAV_BBR_SFDR = 0x0800,    // SFDR Parameters (only available on the ADR/UDR/HPS product variant) and weak signal compensation estimates
  NAV_BBR_VMON = 0x1000,    // SFDR Vehicle Monitoring Parameter (only available on the ADR/UDR/HPS product variant)
  NAV_BBR_TCT = 0x2000,     // TCT Parameters (only available on the ADR/UDR/HPS product variant)

  NAV_BBR_AOP = 0x8000,     // Autonomous orbit parameters
} NAV_BBR;

typedef enum RESET_MODE : uint8_t
{
  RESET_MODE_HW = 0x00,       // immediate reset
  RESET_MODE_SW = 0x01, 
  RESET_MODE_SW_GNSS = 0x02,
  RESET_MODE_HW_SDWN = 0x04,  // after shutdown
  RESET_MODE_GNSS_STOP = 0x08,
  RESET_MODE_GNSS_START = 0x09,
} RESET_MODE;

typedef struct ubx_cfg_rst_t
{
  NAV_BBR     nav_bbr_mask;
  RESET_MODE  reset_mode;
  uint8_t     reserved;

  ubx_cfg_rst_t()
  {
    nav_bbr_mask = NAV_BBR_HOT_START;
    reset_mode = RESET_MODE_HW;
    reserved = 0;
  }
} ubx_cfg_rst_t;

#endif // _UBX_CFG_RST