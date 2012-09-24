#include <dTxfrUSB.h>

#include <EEPROM.h>
#include <SPI.h>

// http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Talk to the sensor on digital pin 2
OneWire oneWire(2);

// http://milesburton.com/Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

DallasTemperature Sensors(&oneWire);

float Celcius, Farenheit;

dTxfrUSB dT;

void setup()
{
  /*
    If you start transmitting instantly on startup, it can be almost 
    impossible to upload again.
  */
  delay(5000);
  Serial.begin(9600);
}

void loop()
{
  dT.Run();
  
  delay(100);       
  
  Sensors.requestTemperatures();
  Celcius = Sensors.getTempCByIndex(0);
  dT.R[0] = Celcius * 10;
  Farenheit = Celcius * 9 / 5 + 32;
  dT.R[1] = Farenheit * 10;
  
  // Serial.print(Celcius); Serial.print("C"); Serial.print("  /  "); Serial.print(Farenheit); Serial.println("F");  
} 














