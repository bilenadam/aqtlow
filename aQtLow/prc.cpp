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

#include "prc.h"
#include <QSettings>
#include <QDebug>

PRC::PRC()
{
}

void PRC::Init(QString Path, int PrcNum)
{
    QString Filename = Path + "/" + QString::number(PrcNum) + ".prc.ini";
    QSettings Settings(Filename, QSettings::IniFormat);
    for(int i = 0; i < NUMBER_OF_REGISTERS; i++)
    {
        Settings.beginGroup("R" + QString::number(i));
        if(Settings.value("Name").toString().length() > 0)
        {
            R[i].Name = Settings.value("Name").toString();
            R[i].Description = Settings.value("Description").toString();
            R[i].Units = Settings.value("Units").toString();
            R[i].Page = Settings.value("Page").toString();
            R[i].PrcLow = Settings.value("PrcLow").toDouble();
            R[i].PrcHigh = Settings.value("PrcHigh").toDouble();
            R[i].IntfcLow = Settings.value("IntfcLow").toDouble();
            R[i].IntfcHigh = Settings.value("IntfcHigh").toDouble();
            R[i].IntfcMin = Settings.value("IntfcMin").toDouble();
            R[i].IntfcMax = Settings.value("IntfcMax").toDouble();
            R[i].LogFilter = Settings.value("LogFilter").toDouble();
            R[i].IsLogged = Settings.value("IsLogged").toInt();
            R[i].IsConfigured = ( R[i].PrcHigh > R[i].PrcLow && R[i].IntfcHigh > R[i].IntfcLow );
        }
        Settings.endGroup();
    }
    for(int i = 0; i < NUMBER_OF_COILS; i++)
    {
        Settings.beginGroup("C" + QString::number(i));
        if(Settings.value("Name").toString().length() > 0)
        {
            C[i].Name = Settings.value("Name").toString();
            C[i].Description = Settings.value("Description").toString();
            C[i].Page = Settings.value("Page").toString();
            C[i].IsLogged = Settings.value("IsLogged").toInt();
            C[i].AlertIf = Settings.value("AlertIf").toInt();
            C[i].IsConfigured = true;
        }
        Settings.endGroup();
    }
    Active = true;
    Settings.disconnect();
}

void PRC::CommFailure()
{
    if(CommOk) CommLost = true;
    CommOk = false;
}

void PRC::CommSuccess()
{
    CommOk = true;
    if(Spinner >= 9)
    {
        Spinner = 0;
    }
    else
    {
        Spinner++;
    }
}
