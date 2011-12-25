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

#include "register.h"
#include "math.h"

REGISTER::REGISTER(QObject *parent) :
        QObject(parent)
{
}

void REGISTER::FromPrc(double PrcRead)
{
    PrcValue = PrcRead;
    double NewValue = Rescale(PrcValue, PrcLow, PrcHigh, IntfcLow, IntfcHigh);
    if(Value != NewValue || !FirstRead)
    {
        FirstRead = true;
        Value = NewValue;
        Text = QString::number(Value);
        emit Changed();
        emit Changed(Value);
        emit Changed(Text);
    }
}

double REGISTER::ValueRounded(int Decimals)
{
    double Multiplier = pow(10, Decimals);
    double A = Value * Multiplier;
    qint64 B = qRound64(A);
    double C = B / Multiplier;
    return C;
}

QString REGISTER::TextRounded(int Decimals)
{
    double Multiplier = pow(10, Decimals);
    double A = Value * Multiplier;
    qint64 B = qRound64(A);
    double C = B / Multiplier;
    return QString::number(C);
}

void REGISTER::Set(double WriteValue)
{
    CombinedWrite(WriteValue);
}

void REGISTER::Set(QString Text)
{
    CombinedWrite(Text.toDouble());
}

void REGISTER::Reset()
{
    CombinedWrite(0);
}

void REGISTER::Inc(double Increment)
{
    CombinedWrite(Value + Increment);
}

void REGISTER::Dec(double Decrement)
{
    CombinedWrite(Value - Decrement);
}

void REGISTER::CombinedWrite(double WriteValue)
{    
    if(!WriteRequest)
    {
        WriteValue = Clamp(WriteValue, IntfcMin, IntfcMax);
        PrcWrite = Rescale(WriteValue, IntfcLow, IntfcHigh, PrcLow, PrcHigh);
        if(PrcWrite != PrcValue)
        {
            WriteRequest = true;
        }
    }
}

double Rescale(double In, double InLow, double InHigh, double OutLow, double OutHigh)
{
    double InRatio = 0;
    bool Flip = false;
    if(InLow > InHigh)
    {
        double FlipIn = InHigh;
        InHigh = OutLow;
        InLow = FlipIn;
        Flip = true;
    }
    In = Clamp(In, InLow, InHigh);
    if(InHigh > InLow) InRatio = (In - InLow) / (InHigh - InLow);
    if(Flip)
    {
        InRatio = 1 - InRatio;
        Flip = false;
    }
    if(OutLow > OutHigh)
    {
        double FlipOut = OutHigh;
        OutHigh = OutLow;
        OutLow = FlipOut;
        Flip = true;
    }
    if(Flip)
    {
        InRatio = 1 - InRatio;
        Flip = false;
    }
    return InRatio * (OutHigh - OutLow) + OutLow;
}

double Clamp(double In, double Low, double High)
{
    if(Low > High)
    {
        double Flip = High;
        High = Low;
        Low = Flip;
    }
    if(In < Low) In = Low;
    if(In > High) In = High;
    return In;
}
