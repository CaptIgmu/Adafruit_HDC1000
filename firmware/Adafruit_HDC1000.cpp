/*************************************************** 
  This is a library for the HDC1000 Humidity & Temp Sensor

  Designed specifically to work with the HDC1008 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
  
  Modified for Photon  needs application.h for types  RMB
 ****************************************************/
#include "application.h"
#include "Adafruit_HDC1000/Adafruit_HDC1000.h"


Adafruit_HDC1000::Adafruit_HDC1000() {
}


boolean Adafruit_HDC1000::begin(uint8_t addr) {
  _i2caddr = addr;

  Wire.begin();
  
  reset();
  if (read16(HDC1000_MANUFID) != 0x5449) return false;
  if (read16(HDC1000_DEVICEID) != 0x1000) return false;
  return true;
}



void Adafruit_HDC1000::reset(void) {
  // reset,combined temp/humidity measurement,  and select 14 bit temp & humidity resolution
  uint16_t config = HDC1000_CONFIG_RST | HDC1000_CONFIG_MODE | HDC1000_CONFIG_TRES_14 | HDC1000_CONFIG_HRES_14;

  Wire.beginTransmission(_i2caddr);
  Wire.write(HDC1000_CONFIG);   // set pointer register to configuration register   RMB
  Wire.write(config>>8);        // now write out 2 bytes MSB first    RMB
  Wire.write(config&0xFF);
  Wire.endTransmission();
  delay(15);
}


float Adafruit_HDC1000::readTemperature(void) {
  
  float temp = (read32(HDC1000_TEMP, 20) >> 16);
  temp /= 65536;
  temp *= 165;
  temp -= 40;

  return temp;
}
  

float Adafruit_HDC1000::readHumidity(void) {
  // original code used HDC1000_TEMP register, doesn't work with HDC1000_HUMID with 32bit read  RMB
  float hum = (read32(HDC1000_TEMP, 20) & 0xFFFF);

  hum /= 65536;
  hum *= 100;

  return hum;
}

// Add ability to test battery voltage, useful in remote monitoring, TRUE if <2.8V  
// Thanks to KFricke for micropython-hdc1008 on GitHub, usually called after Temp/Humid reading  RMB
bool Adafruit_HDC1000::batteryLOW(void)  {
  
  uint16_t battV = (read16(HDC1000_CONFIG_BATT, 20));
  
  battV &= HDC1000_CONFIG_BATT;   // mask off other bits
  
  return bool battV;
}  
  


/*********************************************************************/

uint16_t Adafruit_HDC1000::read16(uint8_t a, uint8_t d) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(a);
  Wire.endTransmission();
  delay(d);
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
  uint16_t r = Wire.read();
  r <<= 8;
  r |= Wire.read();
  //Serial.println(r, HEX);
  return r;
}

uint32_t Adafruit_HDC1000::read32(uint8_t a, uint8_t d) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(a);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)4);
  uint32_t r = Wire.read();
  r <<= 8;
  r |= Wire.read();
  r <<= 8;
  r |= Wire.read();
  r <<= 8;
  r |= Wire.read();
  //Serial.println(r, HEX);
  return r;
}
