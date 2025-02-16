#ifndef _UBXGPS
#define _UBXGPS

//////////////////////////////////////////////////////////////////////////////////
// Categories
#define CAT_CFG   0x06
#define CAT_ACK   0x05

//////////////////////////////////////////////////////////////////////////////////
// CAT_CFG
#define CFG_NAV5  0x24
#define CFG_TP5   0x31

//////////////////////////////////////////////////////////////////////////////////
// CAT_ACK
#define ACK_ACK   0x01 
#define ACK_NACK  0x00 

#define ACK_TIMEOUT_MS 3000

typedef enum ACK_RESULT : uint8_t
{
  ACK_RESULT_TIMEOUT,
  ACK_RESULT_FALSE,
  ACK_RESULT_TRUE,
} ACK_RESULT;

typedef enum ACK_STATE : uint8_t
{
  ACK_STATE_NEW,
  ACK_STATE_CLASS,
  ACK_STATE_ACK,
  ACK_STATE_ID_LEN,
  ACK_STATE_ID,
  ACK_STATE_CSUM,
} ACK_STATE;

const static uint8_t sync_ubx_chars[] = {0xB5, 0x62};

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

typedef struct ack_state_t
{
  ack_state_t(const msg_id_t& _msg_id)
  {
    msg_id = _msg_id;
    csum = checksum_t();
    state = 0;
    is_ack = false;
  }

  msg_id_t msg_id;
  checksum_t csum;
  uint8_t state;
  bool is_ack;

  ack_state_t process_char(uint8_t c)  
  {
    switch(state)
    {
      case 0:
        if (sync_ubx_chars[0] != c)
          return ack_state_t(msg_id);
        break;

      case 1:
        if (sync_ubx_chars[1] != c)
          return ack_state_t(msg_id);
        break;
      
      case 2:
        if (c != CAT_ACK)
          return ack_state_t(msg_id);
        break;

      case 3:
        if (c == ACK_NACK)
          is_ack = false;
        else if (c == ACK_ACK)
          is_ack = true;
        else
          return ack_state_t(msg_id);
        break;

      case 4:
        break;
    }

    state++;
    return *this;
  }

} ack_state_t;

class UBXGPS
{
public:
  UBXGPS()
  {
    _gps_stream = NULL;
  }

public:
  void init(Stream* stream)
  {
    _gps_stream = stream;
  }

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

private:
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

  ACK_RESULT get_ubx_ack(const msg_id_t& id)
  {
    ACK_STATE state = ACK_STATE_NEW;
    unsigned long start_time = millis();
    do
    {
      if (_gps_stream->available())
      {
        _gps_stream->read();
      }
      switch(state)
      {
        case ACK_STATE_NEW:
          break;
      }
    } while ((millis() - start_time) < ACK_TIMEOUT_MS);
    return ACK_RESULT_TIMEOUT;
  }



private:
  Stream* _gps_stream;
};

#endif // _UBXGPS