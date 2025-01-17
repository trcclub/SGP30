#pragma once
//
//    FILE: SGP30.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
//    DATE: 2021-06-24
// PURPOSE: SGP30 library for Arduino
//     URL: https://github.com/RobTillaart/SGP30
//          https://www.adafruit.com/product/3709


#include "Arduino.h"
#include "Wire.h"

#define SGP30_LIB_VERSION         (F("0.1.0"))

#define SGP30_OK                  0x00


class SGP30
{
public:
  explicit SGP30(TwoWire *wire = &Wire);

#if defined (ESP8266) || defined(ESP32)
  bool     begin(uint8_t sda, uint8_t scl);
#endif
  bool     begin();
  bool     isConnected();
  //  WARNING resets all I2C devices that support this call !!
  void     GenericReset();


  // META
  bool     getID();
  uint16_t getFeatureSet();
  bool     measureTest();   // need a better name


  //  MEASUREMENTS
  //  lastMeasurement works only with measure();
  //                 not with async calls.
  uint32_t lastMeasurement() { return _lastTime; };
  // all == false ==> only TVOC and eCO2
  // blocks 15 - 40 msec  (depends on param all).
  bool     measure(bool all = false);
  
  // async interface
  void     request();
  bool     read();
  void     requestRaw();
  bool     readRaw();

  // get the data
  uint16_t getTVOC()    { return _tvoc; };
  uint16_t getCO2()     { return _co2; };
  uint16_t getH2()      { return _h2; }
  uint16_t getEthanol() { return _ethanol; };


  // CALIBRATION - read datasheet
  // T  in °C
  // RH == 0..100
  float    setRelHumidity(float T, float RH);  // P10
  // set Abs Hum to 0 to disables it...
  void     setAbsHumidity(float AbsoluteHumidity);

  // TODO NEXT RELEASE
/*
  something like this

  void     setAcqBase() {};   // 0x201e
  void     getAcqBase() {};   // 0x2015
  void     setTVOCBase() {};  // 0x2077
  void     getTVOCBase() {};  // 0x20B3

  split getAcqBase in 2 calls?
  uint16_t getCO2base();
  uint16_t getTVOCbase();
*/


  // MISC
  int      lastError();   // TODO extend in code

  uint8_t  _id[6];        // TODO private ...


private:
  uint8_t  _address;
  int      _error;
  uint32_t _lastTime = 0;
  uint32_t _lastRequest = 0;

  int      _command(uint16_t cmd);
  int      _setCommand(uint16_t cmd, uint16_t val);
  uint8_t  _CRC8(uint16_t val);
  void     _init();

  uint16_t _tvoc;
  uint16_t _co2;
  uint16_t _h2;
  uint16_t _ethanol;

  TwoWire*  _wire;
};

// -- END OF FILE --
