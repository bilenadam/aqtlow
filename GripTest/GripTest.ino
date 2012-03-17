const float VLo       = 0.5;
const float VHi       = 4.1;
const float Range     = 125;
const float NonLinear = 4;

double milliVolts, Ratio, Lbs, Max;
boolean Squeezing;
int OffDelay, Score;
String Facetiousness;

void setup()
{
  /*
    If you start transmitting instantly on startup, it can be almost 
    impossible to upload again.
  */
  delay(5000);
  Serial.begin(9600);
  Serial.println("Ready");    
}

void loop()
{
  delay(100);       
  
  //Read voltage from sensor and make it millivolts
  milliVolts = double( analogRead(A0) ) / 1023 * 5;  
  
  //Sensor is 100Lbs but its at the far end from the hinge
  //Convert millivolts to Lbs
  Ratio = ( milliVolts - VLo ) / ( VHi - VLo );
  Ratio = constrain( Ratio,0,1 );
  Lbs = pow( Ratio, NonLinear ) * Range;
  
  if( Lbs > 0 )
  {
    OffDelay = 0;
    if( !Squeezing )
    {
      Squeezing = 1;
      Max = 0;
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
        switch (Score) {
          case 1:
            Facetiousness = "Are you feeling OK?";
            break;
          case 2:
            Facetiousness = "Better luck next time young lady.";
            break;
          case 3:
            Facetiousness = "It's not a magic lamp.  Don't rub it, squeeze it.";
            break;
          case 4:
            Facetiousness = "Vitamins are in aisle 3.";
            break;
          case 5:
            Facetiousness = "Alrighty then.";
            break;
          case 6:
            Facetiousness = "Pretty good.";
            break;
          case 7:
            Facetiousness = "Outstanding.";
            break;
          case 8:
            Facetiousness = "That hand must get a lot of exercise.";
            break; 
          case 9:
            Facetiousness = "You win.  Huzzah.";
            break;
          case 10:
            Facetiousness = "Now try it just using one hand !";
            break;            
        }      
        if(Score>0)
        {
          Serial.println();
          Serial.print("Max Lbs: "); Serial.print(Max); Serial.print("  ");
          Serial.print("Score: "); Serial.print(Score); Serial.println();
          Serial.print(Facetiousness);        
          Serial.println(); 
        }
      }   
    }          
  }
} 
