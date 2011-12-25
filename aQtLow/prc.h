/*
    aQtLow - an Arduino interface.
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

#ifndef PRC_H
#define PRC_H

#include "register.h"
#include "coil.h"
#include <QString>

#define NUMBER_OF_REGISTERS 96
#define NUMBER_OF_COILS     96

class PRC
{
public:
    PRC();
    REGISTER R[NUMBER_OF_REGISTERS];
    COIL C[NUMBER_OF_COILS];
    bool Active, CommOk, CommLost;
    void Init(QString Path, int PrcNum);
    void CommSuccess();
    void CommFailure();
    int AlertCount, InfoCount, AlarmCount, CatastrophyCount;
    int Spinner;
};

#endif // PRC_H
