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

#include "dtransfertcp.h"

#include <QDebug>
#include <QByteArray>

#include "globals.h"

dtransfertcp::dtransfertcp(QObject *parent) :
    QThread(parent)
{
}

void dtransfertcp::run()
{
    Socket = new QTcpSocket();
    QTime Sleeper;
    while(!Shutdown)
    {
        if(Socket->state() >= Socket->ConnectedState)
        {//the socket is open so check for data
            if(ExpectingResponse > 0)
            {
                if(Socket->waitForReadyRead(5000))
                {
                    Receive();
                }
                else
                {
                    Send(1,0,0); //Function 1 siginfies "gimme data"
                }
                if(ExpectingResponse > 2)
                {
                    qDebug() << "DTxfrTcp " << QString::number(Cfg.DTxfrTcp) << " finds the lack of expected response utterly demoralizing " << Cfg.Address << " " << QDateTime::currentDateTime().toString(TIMESTAMP_FORMAT);
                    P[Cfg.Prc].CommFailure();
                    ExpectingResponse = 0;
                    Socket->close();
                    Sleeper.start();
                    while(Sleeper.elapsed() < 1000) msleep(100);
                }
            }
            else
            {
                if(!WriteRequest())
                {
                    Send(1,0,0); //Function 1 siginfies "gimme data"
                }
            }
        }
        else
        {//need to open the socket
            ExpectingResponse = 0;
            P[Cfg.Prc].CommFailure();
            Socket->abort();  //in hope clearing errors, etc.
            Socket->connectToHost(Cfg.Address, 51141);
            if (Socket->waitForConnected(5000))
            {
                qDebug() << "DTxfrTcp " << QString::number(Cfg.DTxfrTcp) << " connected to " << Cfg.Address << " " << QDateTime::currentDateTime().toString(TIMESTAMP_FORMAT);
            }
            else
            {
                qDebug() << "DTxfrTcp " << QString::number(Cfg.DTxfrTcp) << " failed to connect to " << Cfg.Address << " " << QDateTime::currentDateTime().toString(TIMESTAMP_FORMAT);                
            }
        }
        Sleeper.start();
        while(Sleeper.elapsed() < Cfg.Delay) msleep(25);
    }
}

int dtransfertcp::WriteRequest()
{
    bool Status = false;
    for(int i = 0; i < NUMBER_OF_REGISTERS; i++)
    {
        if(P[Cfg.Prc].R[i].WriteRequest)
        {
            Status = true;
            Send(6, i, P[Cfg.Prc].R[i].PrcWrite); //Function 6 siginfies "write register"
            P[Cfg.Prc].R[i].WriteRequest = false;
            break;
        }
    }
    if(!Status)
    {
        for(int i = 0; i < NUMBER_OF_COILS; i++)
        {
            if(P[Cfg.Prc].C[i].WriteRequest)
            {
                Status = true;
                Send(5, i, P[Cfg.Prc].C[i].PrcWrite); //Function 5 siginfies "write coil"
                P[Cfg.Prc].C[i].WriteRequest = false;
                break;
            }
        }
    }
    return Status;
}

void dtransfertcp::Receive()
{
    QByteArray Bytes;
    int CoilPosition, CoilMask;
    bool bolValue;
    quint8 H, L;
    qint16 Data;
    for(int i = 0; i < NUMBER_OF_REGISTERS; i++)
    {
        Bytes = Socket->read(2);
        H = Bytes[0];
        L = Bytes[1];
        Data = 0;
        Data |= H << 8;
        Data |= L;
        if((P[Cfg.Prc].R[i].IsConfigured) && (i < NUMBER_OF_REGISTERS )) P[Cfg.Prc].R[i].FromPrc(Data);
    }
    for(int i = 0; i < NUMBER_OF_COILS/8; i++)
    {
        Bytes = Socket->read(1);
        for(int j = 0; j < 8; j++)
        {//coil address is starting coil + intCoilPosition
            CoilPosition = i * 8 + j;
            if((P[Cfg.Prc].C[CoilPosition].IsConfigured) && (CoilPosition < NUMBER_OF_COILS))
            {
                CoilMask = 1<<j;
                bolValue = (Bytes[0] & CoilMask) > 0;
                P[Cfg.Prc].C[CoilPosition].FromPrc(bolValue);
            }
        }
    }
    Socket->flush();
    ExpectingResponse = 0;
    P[Cfg.Prc].CommSuccess();
}

void dtransfertcp::Send(int Function, int Position, int Data)
{
    QByteArray Bytes;
    Bytes[0] = Function / 256; //Function high
    Bytes[1] = Function % 256; //Function low
    Bytes[2] = Position / 256; //Position high
    Bytes[3] = Position % 256; //Position low
    Bytes[4] = Data     / 256; //Data high
    Bytes[5] = Data     % 256; //Data low
    Socket->write(Bytes, 6);
    Socket->waitForBytesWritten(5000);
    if(Function == 1) ExpectingResponse++;
}

void dtransfertcp::Init(QString Path, int DTxfrTcpNum)
{
    Cfg.DTxfrTcp = DTxfrTcpNum;
    QString Filename = Path + "/" + QString::number(Cfg.DTxfrTcp) + ".dtxfrtcp.ini";
    QSettings Settings(Filename, QSettings::IniFormat);
    Settings.beginGroup("Config");
    Cfg.Address = Settings.value("Address").toString();
    Cfg.Delay = Settings.value("Delay").toInt();
    Cfg.Prc = Settings.value("Processor").toInt();
    Settings.endGroup();
    Settings.disconnect();
}
