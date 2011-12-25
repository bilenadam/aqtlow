#include <EEPROM.h>
#include <SPI.h>

#include <dTxfrUSB.h>

dTxfrUSB dT;
/*
  dTxfrUSB
  .R[0-95] signed integers
  .C[0-95] bool
  .Run()   run every scan or so to communicate
*/

#define VREF 5.0;

int PressThreshold, TempTweak;
float FilteredA3, FilteredA4;

void setup()
{

  delay(5000);  //Time to open the terminal if needed
  Serial.begin(9600);
    
  PressThreshold = word(EEPROM.read(1),EEPROM.read(2));
  dT.R[14] = PressThreshold;
  
  TempTweak = word(EEPROM.read(3),EEPROM.read(4));
  dT.R[4] = TempTweak;
}

void loop()
{
  dT.Run();
  
  //Analog inputs 0-1023

  //Temperature
  dT.R[0] = analogRead(A3);                          //Raw signal from sensor.
  //TMP36 analog temperature sensor 1=+V 2=Signal 3=Gnd
  //From 0.1V to 2.0V = -40 to 150 C / -40 to 302 F
  double VoltsA3 = double(dT.R[0]) / 1023 * VREF;
  FilteredA3 = FilteredA3 * 0.999 + VoltsA3 * 0.001;
  dT.R[1] = FilteredA3 * 1000 + TempTweak;           //Volts on A3 (x1000 to get digits to the right of the decimal).  
  dT.R[2] = map(dT.R[1], 100, 2000, -400, 1500);     //Degrees C x 10
  dT.R[3] = map(dT.R[1], 100, 2000, -400, 3020);     //Degrees F x 10
  //Update stored tweak if it gets changed
  if(TempTweak != dT.R[4])                              
  {
    TempTweak = dT.R[4];
    EEPROM.write(3, highByte(TempTweak));
    EEPROM.write(4, lowByte(TempTweak));
  }  
  //Detect if unusually warm
  dT.C[3] = dT.R[3] > 1000;
  
  //Pressure
  dT.R[10] = analogRead(A4);                                            //Raw signal from sensor.
  /*
  Pressure sensor wiring: 10KOhm from ground to signal and the sensor from signal to +V
  A nickel weighs 5 grams or 0.176 ounces
  0.110V at 0.704 ounces (4 nickels) less than 4 nickels did nothing
  3.301V at 4.224 ounces (24 nickels) not linear, but good enough for this town
  */
  double VoltsA4 = double(dT.R[10]) / 1023 * VREF;
  FilteredA4 = FilteredA4 * 0.9 + VoltsA4 * 0.1;
  dT.R[11] = FilteredA4 * 1000;                                         //Volts on A4 (x1000 to get digits to the right of the decimal).
  dT.R[12] = map(dT.R[11], 0110, 3301, 0704, 4224) * ( dT.R[11] > 0 );  //Ounces (x1000 to get digits to the right of the decimal).
  //Detect past range
  dT.C[2] = dT.R[11] > 4224;
  //Capture max ounces
  if(dT.R[12] > dT.R[13]) dT.R[13] = dT.R[12];
  //Reset max
  if(dT.C[0])
  {
    dT.C[0] = false;
    dT.R[13] = 0;
  }
  //Detect touching by comparing to threshold
  dT.C[1] = dT.R[12] > dT.R[14];
  //Update stored threshold if it gets changed
  if(PressThreshold != dT.R[14])                              
  {
    PressThreshold = dT.R[14];
    EEPROM.write(1, highByte(PressThreshold));
    EEPROM.write(2, lowByte(PressThreshold));
  }  
 
} 
