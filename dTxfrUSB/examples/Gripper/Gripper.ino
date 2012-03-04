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

int VLo, VHi, Range, NonLinear;
double Ratio;
boolean Squeezing;
int SqueezingOffDelay, Max;

void setup()
{
  /*
    If you start transmitting instantly on startup, it can be almost 
    impossible to upload again.
  */
  delay(5000);
  Serial.begin(9600);
    
  //Load setup from eeprom
  //Sensor: http://www.sparkfun.com/products/8685
  //5Vpin to sensor to A0 to 330K to gnd.
  //Lo=1.0V, Hi=4.1V, Range=125, NL=2.8
  VLo = word(EEPROM.read(0),EEPROM.read(1));
  dT.R[0] = VLo;
  VHi = word(EEPROM.read(2),EEPROM.read(3));
  dT.R[1] = VHi;  
  Range = word(EEPROM.read(4),EEPROM.read(5));
  dT.R[2] = Range;  
  NonLinear = word(EEPROM.read(6),EEPROM.read(7));
  dT.R[3] = NonLinear;
}

void loop()
{
  delay(100); 
    
  dT.Run();
  /*
    dT.R[4] Voltage in mV
    dT.R[5] Force in Lbs
    dT.R[6] Max Lbs during last squeeze
    dT.R[7] Score 0-10pts
  */  

  //Read voltage from sensor and make it millivolts
  dT.R[4] = double(analogRead(A0))/1023*5000;
  
  //Sensor is 100Lbs but its at the far end from the hinge
  //Convert millivolts to Lbs
  //Yes, I know it's not accurate
  if( VHi > VLo )
  {
    Ratio = double( dT.R[4] - VLo ) / double( VHi - VLo );
    Ratio = constrain(Ratio,0,1);
    dT.R[5] = pow(  Ratio, double(NonLinear)/100  ) * Range;
  }
  
  if( dT.R[5] > 0 )
  {
    if( !Squeezing )
    {
      Squeezing = 1;
      SqueezingOffDelay = 0;
      Max = 0;
      dT.R[6] = 0;
      for(int i=0; i<=10; i++)
      {
        dT.C[i] = 0;
      }    
    } 
    if( dT.R[5] > Max )
    {
      Max = dT.R[5];      
    }
  }  
  else
  {
    if( SqueezingOffDelay < 10 )
    {
      SqueezingOffDelay++;
    }
    else
    {
      Squeezing = 0;
      dT.R[6] = Max;
      //Score
      dT.R[7] = double(Max) / double(Range) * 10;
      dT.R[7] = constrain(dT.R[7],0,10);
      dT.C[dT.R[7]] = 1;
    }    
  }

  //Save setup numbers if they change
  if(dT.R[0] != VLo)                              
  {
    VLo = dT.R[0];
    EEPROM.write(0, highByte(VLo));
    EEPROM.write(1, lowByte(VLo));
  }  
  if(dT.R[1] != VHi)                              
  {
    VHi = dT.R[1];
    EEPROM.write(2, highByte(VHi));
    EEPROM.write(3, lowByte(VHi));
  }  
  if(dT.R[2] != Range)                              
  {
    Range = dT.R[2];
    EEPROM.write(4, highByte(Range));
    EEPROM.write(5, lowByte(Range));
  }  
  if(dT.R[3] != NonLinear)                              
  {
    NonLinear = dT.R[3];
    EEPROM.write(6, highByte(NonLinear));
    EEPROM.write(7, lowByte(NonLinear));
  }  
} 
