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

#include <QtGui/QApplication>
#include "mainwindow.h"

#include "dtransferusb.h"
#include "dtransfertcp.h"
#include "logger.h"
#include "alerthandling.h"
#include "globals.h"

#include <QDir>
#include <QDesktopServices>
#include <QSettings>

int main(int argc, char *argv[])
{
    Shutdown = false;

    QApplication a(argc, argv);
    a.setApplicationName("aQtLow");

    //Create default data paths if they don't exist
    QDir Path;
    MainPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/" + a.applicationName() + "Data";
    Path.setPath(MainPath);
    if(!Path.exists()) Path.mkpath(Path.path());
    ConfigPath = MainPath + "/Config";
    Path.setPath(ConfigPath);
    if(!Path.exists()) Path.mkpath(Path.path());
    LogPath = MainPath + "/Log";
    Path.setPath(LogPath);
    if(!Path.exists()) Path.mkpath(Path.path());
    ExportPath = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    WavPath = MainPath + "/Wav";
    Path.setPath(WavPath);
    if(!Path.exists()) Path.mkpath(Path.path());

    //Get page names from ini
    QFile IntfcFile(ConfigPath + "/intfc.ini");
    QSettings IntfcSettings(IntfcFile.fileName(), QSettings::IniFormat);
    IntfcSettings.beginGroup("PageNames");
    PageNames[0] = IntfcSettings.value("0").toString();
    PageNames[1] = IntfcSettings.value("1").toString();
    PageNames[2] = IntfcSettings.value("2").toString();
    PageNames[3] = IntfcSettings.value("3").toString();
    PageNames[4] = IntfcSettings.value("4").toString();
    PageNames[5] = IntfcSettings.value("5").toString();
    PageNames[6] = IntfcSettings.value("6").toString();
    PageNames[7] = IntfcSettings.value("7").toString();
    PageNames[8] = IntfcSettings.value("8").toString();
    PageNames[9] = IntfcSettings.value("9").toString();
    IntfcSettings.endGroup();

    MainWindow w;
    w.showNormal();
    w.setWindowTitle(a.applicationName());
    //Show main window in the center of the screen (or a little up and to the left)
    QDesktopWidget *d = QApplication::desktop();
    int ww = d->width();     // returns desktop width
    int wh = d->height();    // returns desktop height
    w.setGeometry(ww / 3 - w.width()/2, wh / 3 - w.height()/2, w.width(), w.height());

    IntfcSettings.beginGroup("Options");
    w.ShowPage(IntfcSettings.value("DefaultPage").toString());
    if(IntfcSettings.value("WindowMode") == "Maximized") w.showMaximized();
    if(IntfcSettings.value("WindowMode") == "Fullscreen") w.showFullScreen();
    w.EnableSound = IntfcSettings.value("EnableSound") == "1";
    w.SBMode = IntfcSettings.value("StatusBar").toString();
    IntfcSettings.endGroup();
    IntfcSettings.disconnect();

    /*
            dTransfer drivers
    */
    dtransferusb DTxfrUsb[NUMBER_OF_DTXFRUSBS];
    for(int i = 0; i < NUMBER_OF_DTXFRUSBS; i++)
    {
        QFile DTxfrUsbFile(ConfigPath + "/" + QString::number(i) + ".dtxfrusb.ini");
        if(DTxfrUsbFile.exists())
        {
            QSettings DTxfrUsbSettings(DTxfrUsbFile.fileName(), QSettings::IniFormat);
            if(DTxfrUsbSettings.value("Config/Enabled").toBool())
            {
                int j = DTxfrUsbSettings.value("Config/Processor").toInt();
                QFile PrcFile(ConfigPath + "/" + QString::number(j) + ".prc.ini");
                if(PrcFile.exists())
                {
                    P[j].Init(ConfigPath, j);
                    DTxfrUsb[i].Init(ConfigPath, i);
                    DTxfrUsb[i].start(QThread::LowestPriority);
                    QObject::connect(&DTxfrUsb[i], SIGNAL(Refresh()), &w, SLOT(Refresh()));
                }
            }
            DTxfrUsbSettings.disconnect();
        }
    }
    dtransfertcp DTxfrTcp[NUMBER_OF_DTXFRTCPS];
    for(int i = 0; i < NUMBER_OF_DTXFRTCPS; i++)
    {
        QFile DTxfrTcpFile(ConfigPath + "/" + QString::number(i) + ".dtxfrtcp.ini");
        if(DTxfrTcpFile.exists())
        {
            QSettings DTxfrTcpSettings(DTxfrTcpFile.fileName(), QSettings::IniFormat);
            if(DTxfrTcpSettings.value("Config/Enabled").toBool())
            {
                int j = DTxfrTcpSettings.value("Config/Processor").toInt();
                QFile PrcFile(ConfigPath + "/" + QString::number(j) + ".prc.ini");
                if(PrcFile.exists())
                {
                    P[j].Init(ConfigPath, j);
                    DTxfrTcp[i].Init(ConfigPath, i);
                    DTxfrTcp[i].start(QThread::LowestPriority);
                    QObject::connect(&DTxfrTcp[i], SIGNAL(Refresh()), &w, SLOT(Refresh()));
                }
            }
            DTxfrTcpSettings.disconnect();
        }
    }

    Logger Log;
    Log.start(QThread::LowestPriority);

    AlertHandling AlertHandler;
    AlertHandler.start(QThread::LowestPriority);
    QObject::connect(&AlertHandler, SIGNAL(Announce(int,QString,QString)), &w, SLOT(Announce(int,QString,QString)));
    QObject::connect(&w, SIGNAL(AnnounceAvailable()), &AlertHandler, SLOT(AnnounceAvailable()));

    return a.exec();
}
