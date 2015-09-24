/*************************************************** 
  This is an example for the HDC100x Humidity & Temp Sensor

  Designed specifically to work with the HDC1008 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to  
  interface

  Modified for Photon RMB
 ****************************************************/

#include "Adafruit_HDC1000/Adafruit_HDC1000.h"

// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (D1 on Photon)
// Connect SDA to I2C data pin  (D0 on Photon)

Adafruit_HDC1000 hdc = Adafruit_HDC1000();

void setup() {
  Serial.begin(9600);
  Serial.println("HDC100x test");
  
  hdc.begin();
  delay(15);    // let the chip initialize

// Not using   couldn't find sensor
/*  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }  */
  
}


void loop() {
  Serial.print("Temp: "); Serial.print(hdc.readTemperature());
  Serial.print("\t\tHum: "); Serial.println(hdc.readHumidity());
  
  // here is a different approach   RMB
  hdc.ReadTempHumidity();     // one conversion, one read version 
  Serial.print("BothTemp: "); Serial.print(hdc.GetTemperature());
  Serial.print("\t\BothHum: "); Serial.print(hdc.GetHumidity());
  Serial.print("\t\BatteryLOW: ");
  if (hdc.batteryLOW()) Serial.println("TRUE");
  else Serial.println("FALSE");
  delay(500);
}
