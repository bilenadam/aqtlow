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

#include "coil.h"

COIL::COIL(QObject *parent) :
        QObject(parent)
{
}

void COIL::FromPrc(bool FP)
{
    PrcValue = FP;
    if(Value != PrcValue || !FirstRead)
    {
        FirstRead = true;
        Value = PrcValue;
        int AlertState = qAbs(AlertIf) * (( AlertIf > 0 && Value > 0 ) || ( AlertIf < 0 && Value == 0 ));
        if(AlertState != Alert)
        {
            LogAlertChange = AlertState - Alert;
            LogAlertRequest = true;
            if(AlertState) Announce = true;
            Alert = AlertState;
        }
        emit Changed();
        emit Changed(Value);
    }
}

void COIL::Set()
{
    CombinedWrite(true);
}
void COIL::Reset()
{
    CombinedWrite(false);
}
void COIL::Toggle()
{
    CombinedWrite(!Value);
}

void COIL::CombinedWrite(bool FH)
{
    if(!WriteRequest)
    {
        PrcWrite = FH;
        if(PrcWrite != PrcValue)
        {
            WriteRequest = true;
        }
    }
}
