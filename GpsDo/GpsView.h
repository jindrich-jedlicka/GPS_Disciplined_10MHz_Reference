#ifndef _GPS_VIEW
#define _GPS_VIEW

#define STR_LEN(x) ((sizeof(x) / sizeof(char)) - 1)

#define TIME_ROW 0
#define TIME_COL 8
#define TIME_VAL "??"
#define TIME_VAL_LEN STR_LEN(TIME_VAL)
#define TIME_SECTION TIME_VAL ":"
#define TIME_SECTION_LEN STR_LEN(TIME_SECTION)

#define SAT_ROW 0
#define SAT_COL 0
#define SAT_PREFIX "SAT "
#define SAT_PREFIX_LEN STR_LEN(SAT_PREFIX)
#define SAT_VALUE "? "
#define SAT_VALUE_LEN STR_LEN(SAT_VALUE)

#define SAT_INFO_ROW 1
#define SAT_INFO_COL 0
#define SAT_INFO_UNUSED_CHAR ' '
#define SAT_INFO_UNTRACKED_CHAR 'x'

#include <NMEAGPS.h>
#include "GpsLiquidCrystal.h"

class GpsView
{
public:
  GpsView()
  {
    clear();
  }

public:
  void display_data(GpsLiquidCrystal& dsp, const NMEAGPS& nmeaGps)
  {
    const gps_fix& fix = nmeaGps.read();

    if (!_initialized)
    {
      init(dsp);
      _initialized = true;
    }

    display_satelites_info(dsp, nmeaGps);
    display_time(dsp, fix);
    display_sat(dsp, fix.satellites);
  }

  void clear()
  {
    init_time_values();
    _sat_count = INVALID_VAL;
    _sat_info_count = 0;
    _initialized = false;
  }

private:
  void init(GpsLiquidCrystal& dsp)
  {
    dsp.clear();
    display_unknown_time(dsp);
    display_unknown_sat(dsp);
  }

  void display_time(GpsLiquidCrystal& dsp, const gps_fix& fix)
  {
    if (fix.valid.time) 
    {
      uint8_t col = TIME_COL;
      dsp.setCursor(col, TIME_ROW);
      display_time_value(dsp, fix.dateTime.hours, _hour);

      col += TIME_SECTION_LEN;
      dsp.setCursor(col, TIME_ROW);
      display_time_value(dsp, fix.dateTime.minutes, _minute);

      col += TIME_SECTION_LEN;
      dsp.setCursor(col, TIME_ROW);
      display_time_value(dsp, fix.dateTime.seconds, _sec);
    }
    else
    {
      display_unknown_time(dsp);
      init_time_values();
    }
  }

  void display_unknown_time(GpsLiquidCrystal& dsp)
  {
    dsp.setCursor(TIME_COL, TIME_ROW);    
    dsp.print(TIME_SECTION TIME_SECTION TIME_VAL);
  }

  void display_time_value(GpsLiquidCrystal& dsp, const uint8_t value, uint8_t& stored_value)
  {
    if (stored_value != value)
    {
      dsp.print_dec_number(value, TIME_VAL_LEN, '0');
      stored_value = value;
    }
  }

  void init_time_values()
  {
    _sec = INVALID_VAL;
    _minute = INVALID_VAL;
    _hour = INVALID_VAL;
  }

private:
  void display_sat(GpsLiquidCrystal& dsp, const uint8_t value)
  {
    if (_sat_count != value)
    {
      dsp.setCursor(SAT_COL + SAT_PREFIX_LEN, SAT_ROW);      
      dsp.print_dec_number(value, SAT_VALUE_LEN);
        
      _sat_count = value;
    }
  }

  void display_unknown_sat(GpsLiquidCrystal& dsp)
  {
    dsp.setCursor(SAT_COL, SAT_ROW);
    dsp.print(SAT_PREFIX SAT_VALUE);
  }

private:
    void display_satelites_info(GpsLiquidCrystal& dsp, const NMEAGPS& nmeaGps)
    {      
      const uint8_t maxSatelites = DISP_COLS - SAT_INFO_COL;
      dsp.setCursor(SAT_INFO_COL, SAT_INFO_ROW);
      if (nmeaGps.satellites_valid())
      {
        int count = nmeaGps.sat_count;
        if (maxSatelites < count)
          count = maxSatelites;

        for (uint8_t i = 0; i < count; i++)
        {
          const NMEAGPS::satellite_view_t& sat = nmeaGps.satellites[i];
          
          if (sat.tracked)
            dsp.print_bar_val(to_bar_value(sat.snr));
          else
            dsp.print(SAT_INFO_UNTRACKED_CHAR);
        }
        fill_unused_chars(dsp, _sat_info_count);
        _sat_info_count = count;
      }
      else
      {
        fill_unused_chars(dsp, maxSatelites);
        _sat_info_count = 0;
      }
    }

    void fill_unused_chars(GpsLiquidCrystal& dsp, uint8_t count)
    {
        for (uint8_t i = 0; i < count; i++)
        {
          dsp.print(SAT_INFO_UNUSED_CHAR);
        }
    }

    uint8_t to_bar_value(uint8_t snr)
    {
      const uint8_t SCALE_DIV = 10; // assumes that 99 dBHz is theoretical snr, thus high values are bit suppressed
      const uint8_t SCALE_DIV_HALF = SCALE_DIV / 2;

      uint8_t val = snr;
      val = val / SCALE_DIV;
      
      if (SCALE_DIV_HALF < (val % SCALE_DIV))
        val++;

      return (uint8_t)val;
    }

private:
  bool _initialized;
  uint8_t _sec;
  uint8_t _minute;
  uint8_t _hour;

  uint8_t _sat_count;
  uint8_t _sat_info_count;

  const uint8_t INVALID_VAL = 0xFF;
};

#endif // _GPS_VIEW