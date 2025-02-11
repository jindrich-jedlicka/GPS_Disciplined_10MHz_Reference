#ifndef _SATELITE_DATA
#define _SATELITE_DATA

#include <NMEAGPS.h>

class SateliteData
{
public:
  SateliteData()
  {
    clear();
  }

public:
  void set_data(NMEAGPS& gps)
  {
    _fix = gps.read();
    _sat_count = gps.sat_count;
    if (0 < _sat_count)
      memcpy(&_satellites, &(gps.satellites), sizeof(NMEAGPS::satellite_view_t) * _sat_count);
  }

  inline const bool is_data_set() const { return _sat_count != INVALID_COUNT; }
  inline const gps_fix& get_fix() const { return _fix; }
  inline uint8_t get_sat_count() const { return _sat_count; }
  inline const NMEAGPS::satellite_view_t& get_satelite_view(uint8_t index) const { return _satellites[index]; }
  bool satellites_valid() const { return (_sat_count >= _fix.satellites); }
  void clear()
  {
    _fix.init();
    _sat_count = INVALID_COUNT;
    memset(&_satellites, 0, sizeof(NMEAGPS::satellite_view_t) * NMEAGPS_MAX_SATELLITES);
  }

private:
  gps_fix _fix;
  NMEAGPS::satellite_view_t _satellites[ NMEAGPS_MAX_SATELLITES ];
  uint8_t _sat_count; 

  const uint8_t INVALID_COUNT = 0xFF;
};

#endif