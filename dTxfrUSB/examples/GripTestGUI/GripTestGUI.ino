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

float VLo, VHi, Range, NonLin;
float Volts, Ratio, Lbs, Max;
boolean Squeezing;
int OffDelay, Score;

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
  //Lo=0.5V, Hi=4.1V, Range=125, NL=4
  dT.R[0] = word(EEPROM.read(0),EEPROM.read(1));
  VLo     = float(dT.R[0])/1000;
  dT.R[1] = word(EEPROM.read(2),EEPROM.read(3));
  VHi     = float(dT.R[1])/1000;
  dT.R[2] = word(EEPROM.read(4),EEPROM.read(5));
  Range   = float(dT.R[2]);
  dT.R[3] = word(EEPROM.read(6),EEPROM.read(7));
  NonLin  = float(dT.R[3])/100;
}

void loop()
{
  delay(100);       
  
  dT.Run();
  
  //Read voltage from sensor and make it millivolts
  Volts = double( analogRead(A0) ) / 1023 * 5;  
  dT.R[4] = Volts * 1000;
  
  //Sensor is 100Lbs but its at the far end from the hinge
  //Convert Volts to Lbs
  //Yes, I know it's not accurate
  if( VHi > VLo )
  {
    Ratio = ( Volts - VLo ) / ( VHi - VLo );
    Ratio = constrain(Ratio,0,1);
    Lbs = pow( Ratio, NonLin ) * Range;
    dT.R[5] = Lbs;
  }
  
  if( Lbs > 0 )
  {
    OffDelay = 0;
    if( !Squeezing )
    {
      Squeezing = 1;
      Max = 0;
      Score = 0;
      dT.R[6] = Max;
      dT.R[7] = Score; 
    } 
    if( Lbs > Max )
    {
      Max = Lbs;      
    }
  }  
  else
  {
    if( Squeezing )
    {
      if( OffDelay < 2 )
      {
        OffDelay++;
      }
      else
      {
        OffDelay = 0;
        Squeezing = 0;
        Score = Max / Range * 10;
        Score = constrain( Score,0,10 ); 
        dT.R[6] = Max;
        dT.R[7] = Score; 
      }   
    }          
  }  
  
  dT.C[0] = Score==10;  //Tilt
  
  //Save setup numbers if they change
  if( VLo != float(dT.R[0])/1000 )                              
  {
    VLo = float(dT.R[0])/1000;
    EEPROM.write(0, highByte( dT.R[0] ));
    EEPROM.write(1,  lowByte( dT.R[0] ));
    dT.C[1] = 1;  //Saved
  }  
  if( VHi != float(dT.R[1])/1000 )                              
  {
    VHi = float(dT.R[1])/1000;
    EEPROM.write(2, highByte( dT.R[1] ));
    EEPROM.write(3,  lowByte( dT.R[1] ));
    dT.C[1] = 1;  //Saved
  }  
  if( Range != float(dT.R[2]) )                              
  {
    Range = float(dT.R[2]);
    EEPROM.write(4, highByte( dT.R[2] ));
    EEPROM.write(5,  lowByte( dT.R[2] ));
    dT.C[1] = 1;  //Saved
  }  
  if( NonLin != float(dT.R[3])/100 )                              
  {
    NonLin = float(dT.R[3])/100;
    EEPROM.write(6, highByte( dT.R[3] ));
    EEPROM.write(7,  lowByte( dT.R[3] ));
    dT.C[1] = 1;  //Saved
  }  
} 














