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

#include "registerbyname.h"

#include "globals.h"

RegisterByName::RegisterByName(QObject *parent) :
    QObject(parent)
{
}

bool RegisterByName::Found(QString Name)
{
    if(Name.length() > 0)
    {
        for(p = 0; p < NUMBER_OF_PRCS; p++)
        {
            if(P[p].Active)
            {
                for(r = 0; r < NUMBER_OF_REGISTERS; r++)
                {
                    if(P[p].R[r].Name == Name)
                    {
                        R = &P[p].R[r];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool RegisterByName::Found(QString Name, QString Page)
{
    if(Name.length() > 0)
    {
        for(p = 0; p < NUMBER_OF_PRCS; p++)
        {
            if(P[p].Active)
            {
                for(r = 0; r < NUMBER_OF_REGISTERS; r++)
                {
                    if(P[p].R[r].Name == Name && P[p].R[r].Page == Page)
                    {
                        R = &P[p].R[r];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool RegisterByName::Found(QString Name, int Prc)
{
    if(Name.length() > 0)
    {
        for(p = 0; p < NUMBER_OF_PRCS; p++)
        {
            if(P[p].Active)
            {
                for(r = 0; r < NUMBER_OF_REGISTERS; r++)
                {
                    if(P[p].R[r].Name == Name && p == Prc)
                    {
                        R = &P[p].R[r];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
