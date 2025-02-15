#ifndef _CONFIG_RUNTIME_MODULE
#define _CONFIG_RUNTIME_MODULE

#include "RuntimeModule.h"
#include "RuntimeContext.h"

static uint8_t sync_ubx_chars[] = {0xB5, 0x62};

static uint8_t nav_data[] = {0xFF, 0xFF, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

typedef struct checksum_t
{
  checksum_t()
  {
    ck_a = 0;
    ck_b = 0;
  }
  checksum_t& add_value(uint8_t value)
  {
      ck_a += value;
      ck_b += ck_a;
      return *this;
  }

  uint8_t ck_a;
  uint8_t ck_b;
} checksum_t;

typedef struct msg_id_t
{
  msg_id_t()
  {
    msg_class = 0;
    msg_id = 0;
  }
  msg_id_t(uint8_t _msg_class, uint8_t _msg_id)
  {
    msg_class = _msg_class;
    msg_id = _msg_id;
  }

  uint8_t msg_class;
  uint8_t msg_id;
} msg_id_t;

class ConfigRuntimeModule : public RuntimeModule
{
public:
  ConfigRuntimeModule() : RuntimeModule(0)
  {
    _gps_stream = NULL;    
  }

public:
  virtual MODULE_TYPE get_type() { return MODULE_TYPE_CONFIG; }

protected:
  virtual void on_init()
  {
    //_gps_stream = &RuntimeContext::get_gps_stream();
    _gps_stream = &Serial;
  }

  virtual void on_loop()
  {
    msg_id_t id(0x06, 0x24);
    send_ubx_msg(id, sizeof(nav_data), nav_data);
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
  void send_ubx_msg(const msg_id_t& id, const uint16_t data_len, const uint8_t *p_data)
  {
    if (_gps_stream != NULL)
    {
      send_data(sizeof(sync_ubx_chars), sync_ubx_chars, NULL);

      checksum_t csum;
      send_data(sizeof(id), (uint8_t *)&id, &csum);
      send_data(sizeof(data_len), (uint8_t *)&data_len, &csum);
      send_data(data_len, p_data, &csum);

      send_data(sizeof(csum), (uint8_t *)&csum, NULL);
    }
  }

  void send_data(const uint16_t data_len, const uint8_t *p_data, checksum_t *p_csum)
  {
    for (uint16_t i = 0; i < data_len; i++) 
    {
      uint8_t c = p_data[i];

      //_gps_stream->write(c);
      _gps_stream->print(c, HEX);
      _gps_stream->print(' ');

      if (p_csum != NULL)
        p_csum->add_value(c);
    }
  }

  checksum_t calculate_checksum(checksum_t& csum, const uint8_t *data, const uint16_t len) const
  {
    for (uint16_t i = 0; i < len; i++)
    {
      csum.ck_a += data[i];
      csum.ck_b += csum.ck_a;
    }
    return csum;
  }

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
  Stream* _gps_stream;
};

#endif