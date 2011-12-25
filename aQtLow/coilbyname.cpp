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

#include "coilbyname.h"

#include "globals.h"

CoilByName::CoilByName(QObject *parent) :
    QObject(parent)
{
}

bool CoilByName::Found(QString Name)
{
    if(Name.length() > 0)
    {
        for(p = 0; p < NUMBER_OF_PRCS; p++)
        {
            if(P[p].Active)
            {
                for(c = 0; c < NUMBER_OF_COILS; c++)
                {
                    if(P[p].C[c].Name == Name)
                    {
                        C = &P[p].C[c];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool CoilByName::Found(QString Name, QString Page)
{
    if(Name.length() > 0)
    {
        for(p = 0; p < NUMBER_OF_PRCS; p++)
        {
            if(P[p].Active)
            {
                for(c = 0; c < NUMBER_OF_COILS; c++)
                {
                    if(P[p].C[c].Name == Name && P[p].C[c].Page == Page)
                    {
                        C = &P[p].C[c];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool CoilByName::Found(QString Name, int Prc)
{
    if(Name.length() > 0)
    {
        for(p = 0; p < NUMBER_OF_PRCS; p++)
        {
            if(P[p].Active)
            {
                for(c = 0; c < NUMBER_OF_COILS; c++)
                {
                    if(P[p].C[c].Name == Name && p == Prc)
                    {
                        C = &P[p].C[c];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
