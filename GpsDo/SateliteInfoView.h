#ifndef _SATELITE_INFO_VIEW
#define _SATELITE_INFO_VIEW

#include "View.h"

#define MASK_FIRST_ROW  "   ID       dBHz"
#define MASK_SECOND_ROW "ELV   AZM    deg"
#define INVALID_VAL_CHAR '?'

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
#define ELV_COL 3
#define ELV_LEN 2

#define AZM_ROW ELV_ROW
#define AZM_COL 9
#define AZM_LEN 3

#define INVALID_ID 0xFF
#define INVALID_INDEX 0xFF
#define INVALID_SNR 0xFF  
#define INVALID_ELV 0xFF 
#define INVALID_AZM 0xFFFF

class SateliteInfoView : public View
{
public:
  SateliteInfoView()
  {
    _index = INVALID_INDEX;
    _satelite_data.id = INVALID_ID;
  }

protected:
  virtual void on_init(GpsLiquidCrystal& dsp)
  {
      init(dsp);
      print_invalid_data(dsp);
  }

  virtual void on_display_data(GpsLiquidCrystal& dsp, uint8_t index, const SateliteData& gpsData)
  {
    uint8_t sat_index = index - 1;

    if (gpsData.satellites_valid() && sat_index < gpsData.get_sat_count())
      display_valid_data(dsp, index, gpsData.get_satelite_view(sat_index));
    else
      display_unknown_data(dsp, index);
  }

  virtual void on_clear()
  {
    _index = INVALID_INDEX;
    _satelite_data.id = INVALID_ID;
  }

private:
  void display_unknown_data(GpsLiquidCrystal& dsp, uint8_t index)
  {
    print_index(dsp, index);
    if (_satelite_data.id != INVALID_ID) 
    {
      print_invalid_data(dsp);
      _satelite_data.id = INVALID_ID;
    }
  }

  void display_valid_data(GpsLiquidCrystal& dsp, uint8_t index, const NMEAGPS::satellite_view_t& satelite_data)
  {
    print_index(dsp, index);
    print_data(dsp, satelite_data, _satelite_data.id == INVALID_ID);
  }

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
      _satelite_data.id = data.id;
    }
    if (data.tracked)
    {
      if (forceUpdate || data.snr != _satelite_data.snr)
      {
        dsp.setCursor(SNR_COL, SNR_ROW);
        dsp.print_dec_number(data.snr, SNR_LEN);
        _satelite_data.snr = data.snr;
      }
      if (forceUpdate || data.elevation != _satelite_data.elevation)
      {
        dsp.setCursor(ELV_COL, ELV_ROW);
        dsp.print_dec_number(data.elevation, ELV_LEN);
        _satelite_data.elevation = data.elevation;
      }
      if (forceUpdate || data.azimuth != _satelite_data.azimuth)
      {
        dsp.setCursor(AZM_COL, AZM_ROW);
        dsp.print_dec_number(data.azimuth, AZM_LEN);
        _satelite_data.azimuth = data.azimuth;
      }
    }
    else if (forceUpdate || _satelite_data.tracked)
    {
      print_untracked_data(dsp);
    }
    _satelite_data.tracked = data.tracked;
  }

  void print_untracked_data(GpsLiquidCrystal& dsp)
  {
    dsp.setCursor(SNR_COL, SNR_ROW);
    dsp.print_n_char(SNR_LEN, INVALID_VAL_CHAR);
    _satelite_data.snr = INVALID_SNR;

    dsp.setCursor(ELV_COL, ELV_ROW);
    dsp.print_n_char(ELV_LEN, INVALID_VAL_CHAR);
    _satelite_data.elevation = INVALID_ELV;

    dsp.setCursor(AZM_COL, AZM_ROW);
    dsp.print_n_char(AZM_LEN, INVALID_VAL_CHAR);
    _satelite_data.azimuth = INVALID_AZM;
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
};

#endif // _SATELITE_INFO_VIEW