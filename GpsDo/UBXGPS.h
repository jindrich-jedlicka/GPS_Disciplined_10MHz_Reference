#ifndef _UBXGPS
#define _UBXGPS

#include "ubx_cfg_cfg.h"
#include "ubx_cfg_rst.h"

//////////////////////////////////////////////////////////////////////////////////
// Categories
#define CAT_CFG   0x06
#define CAT_ACK   0x05

//////////////////////////////////////////////////////////////////////////////////
// CAT_CFG
#define CFG_RST   0x04
#define CFG_CFG   0x09
#define CFG_ANT   0x13
#define CFG_NAV5  0x24
#define CFG_TP5   0x31
#define CFG_GNSS  0x3E

//////////////////////////////////////////////////////////////////////////////////
// CAT_ACK
#define ACK_ACK   0x01 
#define ACK_NACK  0x00 

#define ACK_TIMEOUT_MS 5000

typedef enum ACK_RESULT : uint8_t
{
  ACK_RESULT_TIMEOUT,
  ACK_RESULT_FALSE,
  ACK_RESULT_TRUE,
} ACK_RESULT;

const static uint8_t sync_ubx_chars[] = {0xB5, 0x62};

//////////////////////////////////////////////////////////////////////////////////
// checksum_t
//////////////////////////////////////////////////////////////////////////////////
typedef struct checksum_t
{
  uint8_t ck_a;
  uint8_t ck_b;
  
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
} checksum_t;

//////////////////////////////////////////////////////////////////////////////////
// msg_id_t
//////////////////////////////////////////////////////////////////////////////////
typedef struct msg_id_t
{
  uint8_t msg_class;
  uint8_t msg_id;

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
} msg_id_t;

//////////////////////////////////////////////////////////////////////////////////
// ack_state_t
//////////////////////////////////////////////////////////////////////////////////
typedef struct ack_state_t
{
  msg_id_t msg_id;
  checksum_t csum;
  uint8_t state;
  bool is_ack;

  ack_state_t(const msg_id_t& _msg_id)
  {
    msg_id = _msg_id;
    state = 0;
    is_ack = false;
  }

public:
  ack_state_t process_char(uint8_t c)  
  {
    switch(state)
    {
      case 0: 
      case 1: 
        return if_equal_move_next(sync_ubx_chars[state], c);

      case 2:
        return if_equal_move_next_incl_csum(CAT_ACK, c);

      case 3:
        if (c == ACK_NACK)
          is_ack = false;
        else if (c == ACK_ACK)
          is_ack = true;
        else
          return ack_state_t(msg_id);

        csum.add_value(c);
        return move_next();

      case 4:
        return if_equal_move_next_incl_csum(2, c); // message length - low byte
      case 5:
        return if_equal_move_next_incl_csum(0, c); // message length - high byte

      case 6:
        return if_equal_move_next_incl_csum(msg_id.msg_class, c);         
      case 7:      
        return if_equal_move_next_incl_csum(msg_id.msg_id, c); 
        
      case 8:
        return if_equal_move_next(csum.ck_a, c);        
      case 9:
        return if_equal_move_next(csum.ck_b, c);

      default:
        return ack_state_t(msg_id);
    }
  }

  inline bool ack_ready() { return state == 10; }

private:
  ack_state_t if_equal_move_next(const uint8_t a, const uint8_t b)  
  {
    if (a != b)
      return ack_state_t(msg_id);

    return move_next();
  }

  ack_state_t if_equal_move_next_incl_csum(const uint8_t a, const uint8_t b)  
  {
    if (a != b)
      return ack_state_t(msg_id);

    csum.add_value(a);
    return move_next();
  }

  inline ack_state_t move_next()
  {
    state++;
    return *this;
  }
} ack_state_t;

//////////////////////////////////////////////////////////////////////////////////
// UBXGPS
//////////////////////////////////////////////////////////////////////////////////
class UBXGPS
{
public:
  UBXGPS()
  {
    _gps_stream = NULL;
  }

public:
  static void print_result(ACK_RESULT result)
  {
    switch (result)
    {
      case ACK_RESULT_TIMEOUT:
        RuntimeContext::get_display().print(" TIMEOUT");
        break;
      case ACK_RESULT_FALSE:
        RuntimeContext::get_display().print(" FAILED");
        break;
      case ACK_RESULT_TRUE:
        RuntimeContext::get_display().print(" OK");
        break;
    }
  }

  void init(Stream* stream)
  {
    _gps_stream = stream;
  }

  void hw_reset()
  {
    ubx_cfg_rst_t rst_data;
    rst_data.nav_bbr_mask = NAV_BBR_COLD_START;
    rst_data.reset_mode = RESET_MODE_HW;
    send_msg_no_ack(msg_id_t(CAT_CFG, CFG_RST), sizeof(rst_data), (uint8_t *)&rst_data);    
  }

  ACK_RESULT save_all_to_bbr()
  {
    ubx_cfg_cfg_t cfg_data;
    cfg_data.save_mask = CFG_MASK_ALL;
    cfg_data.device_mask = CFG_DEVICE_BBR | CFG_DEVICE_FLASH;
    return send_msg(msg_id_t(CAT_CFG, CFG_CFG), sizeof(cfg_data), (uint8_t *)&cfg_data);
  }

  ACK_RESULT send_msg(const msg_id_t& id, const uint16_t data_len_1, const uint8_t *p_data_1, const uint16_t data_len_2, const uint8_t *p_data_2)
  {
    if (_gps_stream != NULL)
    {
      send_data(sizeof(sync_ubx_chars), sync_ubx_chars, NULL);

      checksum_t csum;
      send_data(sizeof(id), (uint8_t *)&id, &csum);
      uint16_t data_len = data_len_1 + data_len_2;
      send_data(sizeof(data_len), (uint8_t *)&data_len, &csum);
      send_data(data_len_1, p_data_1, &csum);
      send_data(data_len_2, p_data_2, &csum);

      send_data(sizeof(csum), (uint8_t *)&csum, NULL);
      return get_ack(id);
    }
  }

  ACK_RESULT send_msg(const msg_id_t& id, const uint16_t data_len, const uint8_t *p_data)
  {
    if (_gps_stream != NULL)
    {
      send_msg_no_ack(id, data_len, p_data);
      return get_ack(id);
    }
  }

private:
  void send_msg_no_ack(const msg_id_t& id, const uint16_t data_len, const uint8_t *p_data)
  {
      send_data(sizeof(sync_ubx_chars), sync_ubx_chars, NULL);

      checksum_t csum;
      send_data(sizeof(id), (uint8_t *)&id, &csum);
      send_data(sizeof(data_len), (uint8_t *)&data_len, &csum);
      send_data(data_len, p_data, &csum);

      send_data(sizeof(csum), (uint8_t *)&csum, NULL);
  }

  void send_data(const uint16_t data_len, const uint8_t *p_data, checksum_t *p_csum)
  {
    for (uint16_t i = 0; i < data_len; i++) 
    {
      uint8_t c = p_data[i];

      _gps_stream->write(c);
      if (p_csum != NULL)
        p_csum->add_value(c);
    }
  }

  ACK_RESULT get_ack(const msg_id_t& id)
  {
    unsigned long start_time = millis();
    ack_state_t ack = ack_state_t(id);

    do
    {
      if (_gps_stream->available())
      {
        ack = ack.process_char(_gps_stream->read());

        if (ack.ack_ready())
          return ack.is_ack ? ACK_RESULT_TRUE : ACK_RESULT_FALSE;
      }
    } while ((millis() - start_time) < ACK_TIMEOUT_MS);

    return ACK_RESULT_TIMEOUT;
  }

private:
  Stream* _gps_stream;
};

#endif // _UBXGPS