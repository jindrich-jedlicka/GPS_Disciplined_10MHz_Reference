#ifndef _SATELITE_INFO_VIEW
#define _SATELITE_INFO_VIEW

#include <NMEAGPS.h>
#include "GpsLiquidCrystal.h"

#define MASK_FIRST_ROW  "   ID       dBHz"
#define MASK_SECOND_ROW "ELV   AZM    deg"

#define INDEX_ROW 0
#define INDEX_COL 0
#define INDEX_LEN 2

#define ID_ROW INDEX_ROW
#define ID_COL 3
#define ID_LEN 3

class SateliteInfoView
{
public:
  SateliteInfoView()
  {
    clear();
  }

public:
  void display_data(GpsLiquidCrystal& dsp, uint8_t index, const NMEAGPS::satellite_view_t* p_satelite_data)
  {
    if (!_initialized)
    {
      init(dsp);
      _initialized = true;
    }

    if (_index != index)
    {
      dsp.setCursor(INDEX_COL, INDEX_ROW);
      dsp.print_dec_number(index, INDEX_LEN);
      dsp.print(':');

      _index = index;
    }

    if (_satelite_data.id == INVALID_ID)
    {
      // dsp.setCursor(
      // dsp.print_dec_number();
    }
  }

  void clear()
  {
    _index = INVALID_INDEX;
    _satelite_data.id = INVALID_ID;
    _initialized = false;
  }

private:
  void init(GpsLiquidCrystal& dsp)
  {
    dsp.clear();
  }


private:
  uint8_t _index;
  NMEAGPS::satellite_view_t _satelite_data;
  bool _initialized;

  const uint8_t INVALID_ID = 0xFF;  
  const uint8_t INVALID_INDEX = 0xFF;  
};

#endif // _SATELITE_INFO_VIEW