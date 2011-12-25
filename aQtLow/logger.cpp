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

#include "logger.h"
#include <QDebug>
#include <QSettings>

Logger::Logger(QObject *parent) :
    QThread(parent)
{
}

void Logger::run()
{
    LoadConfig();
    QTime ScanTime, PurgeTime;
    qint64 SinceLast;
    bool Different;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString Filename = LogPath + "/Log.sqlite";
    db.setDatabaseName(Filename);
    ScanTime.start();
    PurgeTime.start();
    while(!Shutdown)
    {
        if(!db.isOpen())
        {
            if(!db.open())
            {
                qDebug() << "Failed to open log " << Filename << " " << QDateTime::currentDateTime().toString(TIMESTAMP_FORMAT);
                sleep(10);
            }
            else
            {
                qDebug() << "Opened log " << Filename << " Hours=" << QString::number(SaveTime) << " " << QDateTime::currentDateTime().toString(TIMESTAMP_FORMAT);
                QSqlQuery Query;
                Query.exec("create table Notes (Timestamp, Note)");
                Query.exec("create table Vals (Timestamp, Page, Name, Value)");
                Query.exec("create table Alerts (Timestamp, Page, Name, Alert, Description)");
            }
        }
        else
        {
            if(LogNote.WriteRequest)
            {
                QSqlQuery Query;
                Query.exec("insert into Notes values( '" + Timestamp() + "', '" + LogNote.Text + "')");
                LogNote.Reset();
            }
            for(int p = 0; p < NUMBER_OF_PRCS; p++)
            {
                if(P[p].Active)
                {
                    for(int r = 0; r < NUMBER_OF_REGISTERS; r++)
                    {
                        REGISTER& R = P[p].R[r];
                        SinceLast = QDateTime::currentMSecsSinceEpoch() - R.LastLogTime;
                        Different = (fabs(R.Value - R.LastLogValue) > R.LogFilter) && (R.Value != R.LastLogValue);
                        if(((Different && LogChanges) || ((SinceLast > MaxTime) && (MaxTime > 0))) && (SinceLast > MinTime) && R.IsLogged)
                        {
                            QSqlQuery Query;
                            Query.exec("insert into Vals values( '" + Timestamp() + "', '" + R.Page + "', '" + R.Name + "', '" + QString::number(R.Value) + "')");
                            R.LastLogTime = QDateTime::currentMSecsSinceEpoch();
                            R.LastLogValue = R.Value;
                            if(!Query.isActive()) db.close();
                        }
                    }
                    for(int c = 0; c < NUMBER_OF_COILS; c++)
                    {
                        COIL& C = P[p].C[c];
                        SinceLast = QDateTime::currentMSecsSinceEpoch() - C.LastLogTime;
                        Different = C.Value != C.LastLogValue;
                        if(((Different && LogChanges) || ((SinceLast > MaxTime) && (MaxTime > 0))) && (SinceLast > MinTime) && C.IsLogged)
                        {
                            QSqlQuery Query;
                            Query.exec("insert into Vals values( '" + Timestamp() + "', '" + C.Page + "', '" + C.Name + "', '" + QString::number(C.Value) + "')");
                            C.LastLogTime = QDateTime::currentMSecsSinceEpoch();
                            C.LastLogValue = P[p].C[c].Value;
                            if(!Query.isActive()) db.close();
                        }
                        if(C.LogAlertRequest)
                        {
                            QSqlQuery Query;
                            Query.exec("insert into Alerts values( '" + Timestamp() + "', '" + C.Page + "', '" + C.Name + "', '" + QString::number(C.LogAlertChange) + "', '" + C.Description + "')");
                            C.LogAlertRequest = false;
                            if(!Query.isActive()) db.close();
                        }
                    }
                }
            }
        }
        if(PurgeTime.elapsed() > 60000)
        {
            Purge();
            PurgeTime.start();
        }
        while(ScanTime.elapsed() < 250)msleep(25);
        ScanTime.start();        
    }
}

QString Logger::Timestamp()
{
    return QDateTime::currentDateTime().toString(TIMESTAMP_FORMAT);
}

void Logger::Purge()
{
    QDateTime RightNow = QDateTime::currentDateTime();
    QDateTime Cutoff = RightNow.addSecs(-SaveTime*3600);
    QSqlQuery Query;
    Query.exec("DELETE FROM Notes WHERE Timestamp < '" + Cutoff.toString(TIMESTAMP_FORMAT) + "'");
    Query.exec("DELETE FROM Vals WHERE Timestamp < '" + Cutoff.toString(TIMESTAMP_FORMAT) + "'");
    Query.exec("DELETE FROM Alerts WHERE Timestamp < '" + Cutoff.toString(TIMESTAMP_FORMAT) + "'");
}

void Logger::LoadConfig()
{
    QString IniFile = ConfigPath + "/intfc.ini", Value;
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("Logger");
    SaveTime = Settings.value("Days", "7").toInt() * 24 + Settings.value("Hours", "12").toInt();
    LogChanges = Settings.value("Changes").toInt();
    MinTime = 1000 * Settings.value("MinTime").toInt();
    MaxTime = 1000 * Settings.value("MaxTime").toInt();
    Value = Settings.value("Folder").toString();
    Settings.endGroup();
    Settings.disconnect();
}
