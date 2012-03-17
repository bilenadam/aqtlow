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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include "configure_prcs.h"
#include "configure_dtxfrusb.h"
#include "configure_dtxfrtcp.h"
#include "configure_intfc.h"
#include "currentalerts.h"
#include "alertlog.h"
#include "notelog.h"
#include "valuelog.h"
#include "about.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool EnableSound;
    QString SBMode;

private:
    Ui::MainWindow *ui;    

signals:
    void AnnounceAvailable();
    void RefreshPages();
    void HidePages();
    void SoundCommand(QString Command);
    void CurrentPage(QString Page);

public slots:
    void Announce(int Level, QString Name, QString Text);
    void PageStatus(QString Status);
    void Refresh();
    void PrintPixmap(QPixmap Pix);
    void ShowPage(QString Page);

private slots:
    void SetupPages();
    void on_actionPage_9_triggered();
    void on_actionPage_8_triggered();
    void on_actionPage_7_triggered();
    void on_actionPage_6_triggered();
    void on_actionPage_5_triggered();
    void on_actionPage_4_triggered();
    void on_actionPage_3_triggered();
    void on_actionPage_2_triggered();
    void on_actionPage_1_triggered();
    void on_actionPage_0_triggered();
    void closeEvent( QCloseEvent* );
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionValue_Log_triggered();
    void on_actionAlert_Log_triggered();
    void on_actionNote_Log_triggered();
    void on_actionStore_a_Note_triggered();
    void on_actionCurrent_Alerts_triggered();
    void on_actionConfigure_INTFC_triggered();
    void on_actionConfigure_PRCs_triggered();
    void Refresh_StatusBar();
    void on_actionWiki_triggered();
    void on_actionConfigure_Serial_Transfer_triggered();
    void on_actionAbout_triggered();
    void on_actionConfigure_TCP_Transfer_triggered();
};

#endif // MAINWINDOW_H
