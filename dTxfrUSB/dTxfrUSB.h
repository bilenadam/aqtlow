/*
    dTxfrUSB - an Arduino library for a communicating with Qt.
    Copyright (C) 2011  Dee Wykoff

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

//Comment out next line if using 1.0
#include "WProgram.h"

#include <SPI.h>

#ifndef dTxfrUSB_h
#define dTxfrUSB_h

#define DT_N_R 96
#define DT_N_C 96

enum DT_FC {
  DT_FC_NONE           = 0,
  DT_FC_REQUEST_DATA   = 1,
  DT_FC_WRITE_COIL     = 5,
  DT_FC_WRITE_REGISTER = 6
};

class dTxfrUSB
{
public:
  dTxfrUSB();
  void Run(); 
  void Send(); 
  bool Active; 
/*
	Variables
		R	96 of them
			0 - 95
			signed integers
		C	96 of them
			bools
*/
  int16_t  R[DT_N_R];
  bool     C[DT_N_C];    
private: 
  unsigned long PreviousActivityTime;
  uint8_t ByteArray[260];
  DT_FC FC;
  void SetFC(int fc);
};

#endif


