#ifndef _UBX_CFG_CFG
#define _UBX_CFG_CFG

typedef enum CFG_MASK : uint32_t
{
  CFG_MASK_ALL = 0xFFFF,
  
  CFG_MASK_IO_PORT = 0x0001,    // Communications port settings. Modifying this sub-section results in an IO system reset. Because of this undefined data may be output for a short period of time after receiving the message.
  CFG_MASK_MSG_CONF = 0x0002,   // Message configuration
  CFG_MASK_INF_MSG = 0x0004,    // INF message configuration
  CFG_MASK_NAV_CONF = 0x0008,   // Navigation configuration
  CFG_MASK_RXM_CONF = 0x0010,   // Receiver Manager configuration

  CFG_MASK_SEN_CONF = 0x0100,   // Sensor interface configuration (not supported in protocol versions less than 19)
  CFG_MASK_RINV_CONF = 0x0200,  // Remote inventory configuration
  CFG_MASK_ANT_CONF = 0x0400,   // Antenna configuration
  CFG_MASK_LOG_CONF = 0x0800,   // Logging configuration
  CFG_MASK_FTS_CONF = 0x1000,   // FTS configuration. Only applicable to the FTS product variant.
} CFG_MASK;

typedef enum CFG_DEVICE : uint8_t
{
  CFG_DEVICE_BBR = 0x01,
  CFG_DEVICE_FLASH = 0x02,
  CFG_DEVICE_EEPROM = 0x04,
  CFG_DEVICE_SPI_FLASH = 0x10,
} CFG_DEVICE;

typedef struct ubx_cfg_cfg_t
{
  CFG_MASK    clear_mask;
  CFG_MASK    save_mask;
  CFG_MASK    load_mask;
  CFG_DEVICE  device_mask;

  ubx_cfg_cfg_t()
  {
    clear_mask = 0;
    save_mask = 0;
    load_mask = 0;
    device_mask = 0;
  }
} ubx_cfg_cfg_t;

#endif // _UBX_CFG_CFG