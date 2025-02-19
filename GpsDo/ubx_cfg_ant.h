#ifndef _UBX_CFG_ANT
#define _UBX_CFG_ANT

#define ANT_DEF_PIN_OCD 14
#define ANT_DEF_PIN_SCD 15
#define ANT_DEF_PIN_SWITCH 16 
#define ANT_PIN_RECONFIG 1 << 15

typedef enum ANT_FLAGS : uint16_t
{
  ANT_FLAGS_SVCS = 0x01,        // Enable antenna supply voltage control signal
  ANT_FLAGS_SCD = 0x02,         // Enable short circuit detection
  ANT_FLAGS_OCD = 0x04,         // Enable open circuit detection
  ANT_FLAGS_PDW_ON_SCD = 0x08,  // Power down antenna supply if short circuit is detected. (only in combination with bit 1)
  ANT_FLAGS_RECOVERY = 0x10,    // Enable automatic recovery from short state
} ANT_FLAGS;

typedef struct ubx_cfg_ant_t
{
  ANT_FLAGS flags;
  uint16_t  pins; // see ublock doc, when 0 it doesn't modify anything

  ubx_cfg_ant_t()
  {
    flags = 0;
    pins = 0;
  }

  static uint16_t to_pins(const uint8_t switch_pin, const uint8_t scd_pin, const uint8_t ocd_pin)
  {
    uint16_t pins = (uint16_t)switch_pin;
    pins |= (((uint16_t)scd_pin) << 5);  
    pins |= (((uint16_t)ocd_pin) << 10);
    pins |= ANT_PIN_RECONFIG;
    return pins;
  }
} ubx_cfg_ant_t;

#endif // _UBX_CFG_ANT