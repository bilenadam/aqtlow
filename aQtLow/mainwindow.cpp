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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QResource>
#include <QSettings>

#include "globals.h"
#include "registerbyname.h"
#include "coilbyname.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->addWidget(ui->SBWidget);

    QTimer *RefreshTimer = new QTimer(this);
    connect(RefreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    RefreshTimer->start(1000);

    SetupPages();

    emit HidePages();
    ui->SBPageStatus->clear();
    ui->SBPrcIcon->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Refresh()
{
    if(Shutdown) return;
    if(ui->statusBar->currentMessage().length() == 0) Refresh_StatusBar();    
    emit RefreshPages();
}

void MainWindow::Refresh_StatusBar()
{
    ui->statusBar->setStyleSheet("background-color: palette(window); color: black");

    emit AnnounceAvailable();
    /***** Status bar PRC indicator *****/
    if(PRCsOk)
    {
        ui->SBPrcIcon->hide();
    }
    else
    {
        ui->SBPrcIcon->show();
        ui->SBPrcIcon->setPixmap(QPixmap(":/Pix/Icons/network-wired.png"));
    }
    /***** Status bar Alert indicators *****/
    if(!GInfoCount)
    {
        ui->SBInfosIcon->hide();
    }
    else
    {
        ui->SBInfosIcon->show();
        ui->SBInfosIcon->setPixmap(QPixmap(":/Pix/Icons/dialog-information-4.png"));
        ui->SBInfosIcon->setToolTip(QString::number(GInfoCount));
    }
    if(!GAlarmCount)
    {
        ui->SBAlarmsIcon->hide();
    }
    else
    {
        ui->SBAlarmsIcon->show();
        ui->SBAlarmsIcon->setPixmap(QPixmap(":/Pix/Icons/dialog-error.png"));
        ui->SBAlarmsIcon->setToolTip(QString::number(GAlarmCount));
    }
    if(!GCatastrophyCount)
    {
        ui->SBCatastrophiesIcon->hide();
    }
    else
    {
        ui->SBCatastrophiesIcon->show();
        ui->SBCatastrophiesIcon->setPixmap(QPixmap(":/Pix/Icons/software-update-urgent-2.png"));
        ui->SBCatastrophiesIcon->setToolTip(QString::number(GCatastrophyCount));
    }
    ui->SBDateTime->setText(QDateTime::currentDateTime().toString("M-d h:mm"));
}

void MainWindow::PageStatus(QString Status)
{
    ui->SBPageStatus->setText(Status);
    QFont Font;
    Font = this->font();
    Font.setBold(true);
    ui->SBPageStatus->setFont(Font);
    ui->SBPageStatus->show();
    emit CurrentPage(Status);
}

void MainWindow::on_actionStore_a_Note_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Store a Note"), tr("Enter text:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) LogNote.Set(text);
}

void MainWindow::on_actionValue_Log_triggered()
{
    static ValueLog *wndValueLog = new ValueLog(this);
    wndValueLog->setModal(false);
    wndValueLog->show();
    QObject::connect(wndValueLog, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionNote_Log_triggered()
{
    static NoteLog *wndNoteLog = new NoteLog(this);
    wndNoteLog->setModal(false);
    wndNoteLog->show();
    QObject::connect(wndNoteLog, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionAlert_Log_triggered()
{
    static AlertLog *wndAlertLog = new AlertLog(this);
    wndAlertLog->setModal(false);
    wndAlertLog->show();
    QObject::connect(wndAlertLog, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionCurrent_Alerts_triggered()
{
    static CurrentAlerts *wndCurrentAlerts = new CurrentAlerts(this);
    wndCurrentAlerts->setModal(false);
    wndCurrentAlerts->show();
    QObject::connect(wndCurrentAlerts, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionConfigure_PRCs_triggered()
{
    static Configure_PRCs *wndConfigure_PRCs = new Configure_PRCs(this);
    wndConfigure_PRCs->setModal(true);
    wndConfigure_PRCs->show();
    QObject::connect(wndConfigure_PRCs, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionConfigure_Serial_Transfer_triggered()
{
    static configure_dtxfrusb *wndConfigure_DTxfrUsbs = new configure_dtxfrusb(this);
    wndConfigure_DTxfrUsbs->setModal(true);
    wndConfigure_DTxfrUsbs->show();
    QObject::connect(wndConfigure_DTxfrUsbs, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionConfigure_TCP_Transfer_triggered()
{
    static configure_dtxfrtcp *wndConfigure_DTxfrTcps = new configure_dtxfrtcp(this);
    wndConfigure_DTxfrTcps->setModal(true);
    wndConfigure_DTxfrTcps->show();
    QObject::connect(wndConfigure_DTxfrTcps, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
}

void MainWindow::on_actionConfigure_INTFC_triggered()
{
    static configure_intfc *wndConfigure_Intfc = new configure_intfc(this);
    wndConfigure_Intfc->setModal(true);
    wndConfigure_Intfc->show();
    QObject::connect(wndConfigure_Intfc, SIGNAL(Screenshot(QPixmap)), this, SLOT(PrintPixmap(QPixmap)));
    QObject::connect(wndConfigure_Intfc, SIGNAL(Announce(int,QString,QString)), this, SLOT(Announce(int,QString,QString)));
    QObject::connect(this, SIGNAL(SoundCommand(QString)), wndConfigure_Intfc, SLOT(LastSoundCommand(QString)));
    QObject::connect(this, SIGNAL(CurrentPage(QString)), wndConfigure_Intfc, SLOT(SetCurrentPaqe(QString)));
}

void MainWindow::on_actionWiki_triggered()
{
    QProcess Process;
    Process.startDetached(ui->actionWiki->toolTip());
}

void MainWindow::on_actionAbout_triggered()
{
    static About *wndAbout = new About(this);
    //Show window in the center of the screen
    QDesktopWidget *d = QApplication::desktop();
    int ww = d->width();     // returns desktop width
    int wh = d->height();    // returns desktop height
    wndAbout->setGeometry(ww / 2 - wndAbout->width()/2, wh / 2 - wndAbout->height()/2, wndAbout->width(), wndAbout->height());
    wndAbout->show();
}

void MainWindow::Announce(int Level, QString Name, QString Text)
{
    QProcess Process;
    CoilByName Cbn;
    QFile NameSound, LevelSound;
    QString Command;    
    ui->statusBar->showMessage(Text, 10000);
    if(Level == 1) ui->statusBar->setStyleSheet(QString("background-color: %1; color: %2").arg("blue").arg("white"));
    if(Level == 2) ui->statusBar->setStyleSheet(QString("background-color: %1; color: %2").arg("orange").arg("black"));
    if(Level >= 3) ui->statusBar->setStyleSheet(QString("background-color: %1; color: %2").arg("red").arg("black"));
    // If a file exists named Alert_COILNAME.Wav then play that.
    // Or, if not, then play Alert_LEVEL.Wav instead.
    // If neither exists, then be quiet.
    if(Cbn.Found(Name)) NameSound.setFileName(WavPath + "/" + "Alert_" + Cbn.C->Name + ".wav");
    LevelSound.setFileName(WavPath + "/" + "Alert_" + QString::number(Level) + ".wav");
    if(EnableSound)
    {
        if(NameSound.exists())
        {
            Command = "vlc --play-and-exit --qt-start-minimized " + NameSound.fileName();
            Process.startDetached(Command);
            emit SoundCommand(Command);
        }
        else
        {
            if(LevelSound.exists())
            {
                Command = "vlc --play-and-exit --qt-start-minimized " + LevelSound.fileName();
                Process.startDetached(Command);
                emit SoundCommand(Command);
            }
        }
    }
}

void MainWindow::on_actionPrint_triggered()
{
    PrintPixmap(QPixmap::grabWidget(this));
}

void MainWindow::PrintPixmap(QPixmap Pix)
{
    QPrinter Printer;
    QPainter Painter;
    QRectF Paper = Printer.paperRect(QPrinter::Inch);
    int XRes = this->width() / (Paper.width() - 1);
    int YRes = this->height() / (Paper.height() - 1);
    if(XRes > YRes)
    {
        Printer.setResolution(XRes);
    }
    else
    {
        Printer.setResolution(YRes);
    }
    Painter.begin(&Printer);
    Painter.drawPixmap(0, 0, Pix.width(), Pix.height(), Pix);
    Painter.end();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent* close)
{
    if(!Shutdown)
    {
        emit HidePages();
        //Don't actually exit yet.
        close->ignore();
        //Tell the threads to stop running thier main loop.
        Shutdown = true;
    }
    //Notification of what is going on.
    ui->statusBar->showMessage("Halting threads and shutting down", 10000);
    ui->statusBar->setStyleSheet(QString("background-color: %1").arg("white"));
    //Wait a while and then close this window for real.
    QTimer *ShutdownTimer = new QTimer(this);
    connect(ShutdownTimer, SIGNAL(timeout()), this, SLOT(close()));
    ShutdownTimer->start(2500);
}

void MainWindow::ShowPage(QString Page)
{
    if(ui->Widget_Page_0->Page == Page) ui->Widget_Page_0->show();
    if(ui->Widget_Page_1->Page == Page) ui->Widget_Page_1->show();
    if(ui->Widget_Page_2->Page == Page) ui->Widget_Page_2->show();
    if(ui->Widget_Page_3->Page == Page) ui->Widget_Page_3->show();
    if(ui->Widget_Page_4->Page == Page) ui->Widget_Page_4->show();
    if(ui->Widget_Page_5->Page == Page) ui->Widget_Page_5->show();
    if(ui->Widget_Page_6->Page == Page) ui->Widget_Page_6->show();
    if(ui->Widget_Page_7->Page == Page) ui->Widget_Page_7->show();
    if(ui->Widget_Page_8->Page == Page) ui->Widget_Page_8->show();
    if(ui->Widget_Page_9->Page == Page) ui->Widget_Page_9->show();
}

void MainWindow::on_actionPage_0_triggered()
{
    emit HidePages();
    ui->Widget_Page_0->show();
}

void MainWindow::on_actionPage_1_triggered()
{
    emit HidePages();
    ui->Widget_Page_1->show();
}

void MainWindow::on_actionPage_2_triggered()
{
    emit HidePages();
    ui->Widget_Page_2->show();
}

void MainWindow::on_actionPage_3_triggered()
{
    emit HidePages();
    ui->Widget_Page_3->show();
}

void MainWindow::on_actionPage_4_triggered()
{
    emit HidePages();
    ui->Widget_Page_4->show();
}

void MainWindow::on_actionPage_5_triggered()
{
    emit HidePages();
    ui->Widget_Page_5->show();
}

void MainWindow::on_actionPage_6_triggered()
{
    emit HidePages();
    ui->Widget_Page_6->show();
}

void MainWindow::on_actionPage_7_triggered()
{
    emit HidePages();
    ui->Widget_Page_7->show();
}

void MainWindow::on_actionPage_8_triggered()
{
    emit HidePages();
    ui->Widget_Page_8->show();
}

void MainWindow::on_actionPage_9_triggered()
{
    emit HidePages();
    ui->Widget_Page_9->show();    
}

void MainWindow::SetupPages()
{
    if(!PageNames[0].length()) PageNames[0] = "Page 0";
    ui->Widget_Page_0->SetPage(PageNames[0]);
    ui->actionPage_0->setText(PageNames[0]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_0, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_0, SLOT(Refresh()));
    connect(ui->Widget_Page_0, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[1].length()) PageNames[1] = "Page 1";
    ui->Widget_Page_1->SetPage(PageNames[1]);
    ui->actionPage_1->setText(PageNames[1]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_1, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_1, SLOT(Refresh()));
    connect(ui->Widget_Page_1, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[2].length()) PageNames[2] = "Page 2";
    ui->Widget_Page_2->SetPage(PageNames[2]);
    ui->actionPage_2->setText(PageNames[2]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_2, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_2, SLOT(Refresh()));
    connect(ui->Widget_Page_2, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[3].length()) PageNames[3] = "Page 3";
    ui->Widget_Page_3->SetPage(PageNames[3]);
    ui->actionPage_3->setText(PageNames[3]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_3, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_3, SLOT(Refresh()));
    connect(ui->Widget_Page_3, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[4].length()) PageNames[4] = "Page 4";
    ui->Widget_Page_4->SetPage(PageNames[4]);
    ui->actionPage_4->setText(PageNames[4]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_4, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_4, SLOT(Refresh()));
    connect(ui->Widget_Page_4, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[5].length()) PageNames[5] = "Page 5";
    ui->Widget_Page_5->SetPage(PageNames[5]);
    ui->actionPage_5->setText(PageNames[5]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_5, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_5, SLOT(Refresh()));
    connect(ui->Widget_Page_5, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[6].length()) PageNames[6] = "Page 6";
    ui->Widget_Page_6->SetPage(PageNames[6]);
    ui->actionPage_6->setText(PageNames[6]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_6, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_6, SLOT(Refresh()));
    connect(ui->Widget_Page_6, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[7].length()) PageNames[7] = "Page 7";
    ui->Widget_Page_7->SetPage(PageNames[7]);
    ui->actionPage_7->setText(PageNames[7]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_7, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_7, SLOT(Refresh()));
    connect(ui->Widget_Page_7, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[8].length()) PageNames[8] = "Page 8";
    ui->Widget_Page_8->SetPage(PageNames[8]);
    ui->actionPage_8->setText(PageNames[8]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_8, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_8, SLOT(Refresh()));
    connect(ui->Widget_Page_8, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));

    if(!PageNames[9].length()) PageNames[9] = "Page 9";
    ui->Widget_Page_9->SetPage(PageNames[9]);
    ui->actionPage_9->setText(PageNames[9]);
    connect(this, SIGNAL(HidePages()), ui->Widget_Page_9, SLOT(hide()));
    connect(this, SIGNAL(RefreshPages()), ui->Widget_Page_9, SLOT(Refresh()));
    connect(ui->Widget_Page_9, SIGNAL(PageStatus(QString)), this, SLOT(PageStatus(QString)));
}


