/*
    dTxfrTCP - an Arduino library for a communicating with Qt.
    Copyright (C) 2012  Dee Wykoff

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dTxfrTCP.h"

Server server(51141);

dTxfrTCP::dTxfrTCP()
{
}

void dTxfrTCP::Run()
{  
/*
	Incoming messages from PC
		Byte	Meaning
		0	Hi byte of function code
		1	Low byte
		2	Hi byte of destination (if a write command)
		3	Low byte
		4	Hi byte of data (if a write command)
		5	Low byte
*/
/*
	Outgoing message to PC
		Byte	Meaning
		0	Hi byte of R[0]
		1	Low byte
		...		
		190	Hi byte of R[95]
		191	Low byte
		192	C[0-7]
		203	C[88-95]
*/
  Client client = server.available();
  uint8_t ByteArray[255];
  int16_t ArrayPosition = 0;
  if(client.available())
  {    
    while(client.available())
    {
      ByteArray[ArrayPosition] = client.read();
      ArrayPosition++;
    }
    if(true)
    {
      SetFC(word(ByteArray[0],ByteArray[1]));
      if(!Active)
      {
        Active = true;
      }
      PreviousActivityTime = millis();
    }  
  }
  if(millis() > (PreviousActivityTime + 60000))
  {
    if(Active)
    {
      Active = false;
    }
  }
  
  //****************** Send Data ***********************
  if(FC == DT_FC_REQUEST_DATA)
  {
    ArrayPosition = 0;
    for(int i = 0; i < DT_N_R; i++)
    {
      ByteArray[ArrayPosition] = highByte(R[i]);
      ArrayPosition++;
      ByteArray[ArrayPosition] = lowByte(R[i]);
      ArrayPosition++;
    }
    uint8_t Byte;
    for(int i = 0; i < (DT_N_C/8); i++)
    {
      Byte = 0;
      for(int j = 0; j < 8; j++)
      {  
        Byte |= (C[i*8+j] << j);
      }
      ByteArray[ArrayPosition] = Byte;
      ArrayPosition++;
    }
    client.write(ByteArray, ArrayPosition);
    FC = DT_FC_NONE;
  }

  //****************** Write Coil **********************
  if(FC == DT_FC_WRITE_COIL)
  {
    C[word(ByteArray[2],ByteArray[3])] = word(ByteArray[4],ByteArray[5]) > 0;
    FC = DT_FC_NONE;
  } 

  //****************** Write Register ******************
  if(FC == DT_FC_WRITE_REGISTER)
  {
    R[word(ByteArray[2],ByteArray[3])] = word(ByteArray[4],ByteArray[5]);
    FC = DT_FC_NONE;
  }  
}

void dTxfrTCP::SetFC(int fc)
{
/*
	Function codes
		Code	Meaning
		1	Gimme data
		5	Write coil C
		6	Write register R
*/
  switch(fc)
  {
    case 1:
      FC = DT_FC_REQUEST_DATA;
      break;
    case 5:
      FC = DT_FC_WRITE_COIL;
      break;
    case 6:
      FC = DT_FC_WRITE_REGISTER;
      break;
    default:
      FC = DT_FC_NONE;
      break;
  }
}
