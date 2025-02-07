#ifndef _SATELITE_INFO_VIEW
#define _SATELITE_INFO_VIEW

#include <NMEAGPS.h>
#include "GpsLiquidCrystal.h"

#define MASK_FIRST_ROW  "   ID       dBHz"
#define MASK_SECOND_ROW "ELV   AZM    deg"
#define INVALID_VAL_CHAR '*'

#define INDEX_ROW 0
#define INDEX_COL 0
#define INDEX_LEN 2

#define ID_ROW INDEX_ROW
#define ID_COL 5
#define ID_LEN 3

#define SNR_ROW INDEX_ROW
#define SNR_COL 10
#define SNR_LEN 2

#define ELV_ROW 1
#define ELV_COL 0
#define ELV_LEN 2

#define AZM_ROW ELV_ROW
#define AZM_COL 0
#define AZM_LEN 9

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
      if (p_satelite_data == NULL)
        print_invalid_data(dsp);

      _initialized = true;
    }

    bool forceUpdate = false;
    print_index(dsp, index);

    if (_satelite_data.id == INVALID_ID) {
      if (p_satelite_data == NULL)
        return;
      forceUpdate = true;
    }
    else if (p_satelite_data == NULL) {      
      print_invalid_data(dsp);
      _satelite_data.id = INVALID_ID;
      return;
    }
    print_data(dsp, *p_satelite_data, forceUpdate);
    _satelite_data = *p_satelite_data;
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

    dsp.setCursor(0, 0);
    dsp.print(MASK_FIRST_ROW);
    dsp.setCursor(0, 1);
    dsp.print(MASK_SECOND_ROW);
  }

  void print_index(GpsLiquidCrystal& dsp, const uint8_t index)
  {
    if (_index != index)
    {
      dsp.setCursor(INDEX_COL, INDEX_ROW);
      dsp.print_dec_number(index, INDEX_LEN);

      _index = index;
    }
  }

  void print_data(GpsLiquidCrystal& dsp, const NMEAGPS::satellite_view_t& data, bool forceUpdate)
  {
    if (forceUpdate || data.id != _satelite_data.id)
    {
      dsp.setCursor(ID_COL, ID_ROW);
      dsp.print_dec_number(data.id, ID_LEN);
    }
    if (data.tracked)
    {
      if (forceUpdate || data.snr != _satelite_data.snr)
      {
        dsp.setCursor(SNR_COL, SNR_ROW);
        dsp.print_dec_number(data.snr, SNR_LEN);
      }
      if (forceUpdate || data.elevation != _satelite_data.elevation)
      {
        dsp.setCursor(ELV_COL, ELV_ROW);
        dsp.print_dec_number(data.elevation, ELV_LEN);
      }
      if (forceUpdate || data.azimuth != _satelite_data.azimuth)
      {
        dsp.setCursor(AZM_COL, AZM_ROW);
        dsp.print_dec_number(data.azimuth, AZM_LEN);
      }
    }
    else if (forceUpdate || _satelite_data.tracked)
    {
      print_untracked_data(dsp);
    }
  }

  void print_untracked_data(GpsLiquidCrystal& dsp)
  {
    dsp.setCursor(SNR_COL, SNR_ROW);
    dsp.print_n_char(SNR_LEN, INVALID_VAL_CHAR);

    dsp.setCursor(ELV_COL, ELV_ROW);
    dsp.print_n_char(ELV_LEN, INVALID_VAL_CHAR);

    dsp.setCursor(AZM_COL, AZM_ROW);
    dsp.print_n_char(AZM_LEN, INVALID_VAL_CHAR);
  }

  void print_invalid_data(GpsLiquidCrystal& dsp)
  {
    dsp.setCursor(ID_COL, ID_ROW);
    dsp.print_n_char(ID_LEN, INVALID_VAL_CHAR);

    print_untracked_data(dsp);
  }

private:
  uint8_t _index;
  NMEAGPS::satellite_view_t _satelite_data;
  bool _initialized;

  const uint8_t INVALID_ID = 0xFF;  
  const uint8_t INVALID_INDEX = 0xFF;  
};

#endif // _SATELITE_INFO_VIEW