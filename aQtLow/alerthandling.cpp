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

#include "alerthandling.h"
#include <QDebug>
#include "globals.h"
#include "QTime"
#include <QSound>

AlertHandling::AlertHandling(QObject *parent) :
    QThread(parent)
{
}

void AlertHandling::run()
{
    QTime ScanTime;
    ScanTime.start();
    AnnouncerBusy = true;
    while(!Shutdown)
    {
        AlertScan();
        while(ScanTime.elapsed() < 500) msleep(50);
        ScanTime.start();
    }
}

void AlertHandling::AlertScan()
{
    int InfTtl = 0, AlmTtl = 0, CatTtl = 0;
    bool OkTest = true;
    for(int p = 0; p < NUMBER_OF_PRCS; p++)
    {
        int InfCnt = 0, AlmCnt = 0, CatCnt = 0;
        if(P[p].Active)
        {
            for(int c = 0; c < NUMBER_OF_COILS; c++)
            {
                switch(P[p].C[c].Alert)
                {
                case 1:
                    InfCnt++;
                    if(P[p].C[c].Announce && !AnnouncerBusy)
                    {
                        AnnouncerBusy = true;
                        emit Announce(1, "", "" + P[p].C[c].Description);
                        P[p].C[c].Announce = false;
                        msleep(50);
                    }
                    break;
                case 2:
                    AlmCnt++;
                    if(P[p].C[c].Announce && !AnnouncerBusy)
                    {
                        AnnouncerBusy = true;
                        emit Announce(2, P[p].C[c].Name, "Alarm : " + P[p].C[c].Name + " : " + P[p].C[c].Description);
                        P[p].C[c].Announce = false;
                        msleep(50);
                    }
                    break;
                case 3:
                    CatCnt++;
                    if(P[p].C[c].Announce && !AnnouncerBusy)
                    {
                        AnnouncerBusy = true;
                        emit Announce(3, P[p].C[c].Name, "Catastrophy : " + P[p].C[c].Name + " : " + P[p].C[c].Description);
                        P[p].C[c].Announce = false;
                        msleep(50);
                    }
                    break;
                }
            }
            P[p].InfoCount = InfCnt;
            P[p].AlarmCount = AlmCnt;
            P[p].CatastrophyCount = CatCnt;
            P[p].AlertCount = InfCnt + AlmCnt + CatCnt;
            InfTtl += InfCnt;
            AlmTtl += AlmCnt;
            CatTtl += CatCnt;
            OkTest = OkTest && P[p].CommOk;
            if(P[p].CommLost && !AnnouncerBusy)
            {
                AnnouncerBusy = true;
                emit Announce(4, "PRC" + QString::number(p), "Processor " + QString::number(p) + " lost");
                P[p].CommLost = false;
                msleep(50);
            }
        }
    }
    GInfoCount = InfTtl;
    GAlarmCount = AlmTtl;
    GCatastrophyCount = CatTtl;
    GAlertCount = GInfoCount + GAlarmCount + GCatastrophyCount;
    PRCsOk = OkTest;
}

void AlertHandling::AnnounceAvailable()
{
    AnnouncerBusy = false;
}
