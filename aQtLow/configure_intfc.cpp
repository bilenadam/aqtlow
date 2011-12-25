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

#include "configure_intfc.h"
#include "ui_configure_intfc.h"

#include <QSettings>
#include <QPrinter>
#include <QPainter>
#include <qdebug.h>

#include "globals.h"

configure_intfc::configure_intfc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configure_intfc)
{
    ui->setupUi(this);

    ui->comboBox_Logger_Changes->addItem("No");
    ui->comboBox_Logger_Changes->addItem("Yes");

    ui->label_Paths_MainPath->setText(MainPath);
    ui->label_Paths_ConfigPath->setText(ConfigPath);
    ui->label_Paths_LogPath->setText(LogPath);
    ui->label_Paths_ExportPath->setText(ExportPath);
    ui->label_Paths_WavPath->setText(WavPath);

    ui->comboBox_Options_WindowMode->addItem("Normal");
    ui->comboBox_Options_WindowMode->addItem("Maximized");
    ui->comboBox_Options_WindowMode->addItem("Fullscreen");
    ui->comboBox_Options_EnableSound->addItem("No");
    ui->comboBox_Options_EnableSound->addItem("Yes");

    for(int p = 0; p < 10; p++)
    {
        ui->comboBox_Options_DefaultPage->addItem(PageNames[p]);
    }

    LoadConfig();
}

configure_intfc::~configure_intfc()
{
    delete ui;
}

void configure_intfc::on_pushButton_Save_clicked()
{

    QString IniFile = ConfigPath + "/intfc.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    switch(ui->tabWidget->currentIndex())
    {
    case 0: //Log
        Settings.beginGroup("Logger");
        Settings.setValue("Hours", ui->spinBox_Logger_Hours->text());
        Settings.setValue("Days", ui->spinBox_Logger_Days->text());
        Settings.setValue("Changes", QString::number(ui->comboBox_Logger_Changes->currentIndex()));
        Settings.setValue("MinTime", QString::number(ui->spinBox_Logger_MinTime->value()));
        Settings.setValue("MaxTime", QString::number(ui->spinBox_Logger_MaxTime->value()));
        Settings.endGroup();
        break;
    case 1: //Paths
        break;
    case 2: //Options
        Settings.beginGroup("Options");
        Settings.setValue("DefaultPage", ui->comboBox_Options_DefaultPage->currentText());
        Settings.setValue("WindowMode", ui->comboBox_Options_WindowMode->currentText());
        Settings.setValue("EnableSound", QString::number(ui->comboBox_Options_EnableSound->currentIndex()));
        Settings.endGroup();
        break;
    case 3: //Page Names
        Settings.beginGroup("PageNames");
        Settings.setValue("0", ui->lineEdit_Page0Name->text());
        Settings.setValue("1", ui->lineEdit_Page1Name->text());
        Settings.setValue("2", ui->lineEdit_Page2Name->text());
        Settings.setValue("3", ui->lineEdit_Page3Name->text());
        Settings.setValue("4", ui->lineEdit_Page4Name->text());
        Settings.setValue("5", ui->lineEdit_Page5Name->text());
        Settings.setValue("6", ui->lineEdit_Page6Name->text());
        Settings.setValue("7", ui->lineEdit_Page7Name->text());
        Settings.setValue("8", ui->lineEdit_Page8Name->text());
        Settings.setValue("9", ui->lineEdit_Page9Name->text());
        Settings.endGroup();
        break;
    }
    Settings.disconnect();
}

void configure_intfc::LoadConfig()
{
    QString IniFile = ConfigPath + "/intfc.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("Logger");
    ui->spinBox_Logger_Hours->setValue(Settings.value("Hours").toInt());
    ui->spinBox_Logger_Days->setValue(Settings.value("Days").toInt());
    ui->comboBox_Logger_Changes->setCurrentIndex(Settings.value("Changes").toInt());
    ui->spinBox_Logger_MinTime->setValue(Settings.value("MinTime").toInt());
    ui->spinBox_Logger_MaxTime->setValue(Settings.value("MaxTime").toInt());
    Settings.endGroup();
    Settings.beginGroup("Options");
    ui->comboBox_Options_DefaultPage->setCurrentIndex( ui->comboBox_Options_DefaultPage->findText(Settings.value("DefaultPage").toString() ) );
    ui->comboBox_Options_WindowMode->setCurrentIndex( ui->comboBox_Options_WindowMode->findText(Settings.value("WindowMode").toString() ) );
    ui->comboBox_Options_EnableSound->setCurrentIndex(Settings.value("EnableSound").toInt());
    ui->pushButton_TestInfoSound->setEnabled(Settings.value("EnableSound").toInt());
    ui->pushButton_TestAlarmSound->setEnabled(Settings.value("EnableSound").toInt());
    ui->pushButton_TestCatastrophySound->setEnabled(Settings.value("EnableSound").toInt());
    ui->pushButton_TestLostSound->setEnabled(Settings.value("EnableSound").toInt());
    Settings.endGroup();
    Settings.beginGroup("PageNames");
    ui->lineEdit_Page0Name->setText(Settings.value("0").toString());
    ui->lineEdit_Page1Name->setText(Settings.value("1").toString());
    ui->lineEdit_Page2Name->setText(Settings.value("2").toString());
    ui->lineEdit_Page3Name->setText(Settings.value("3").toString());
    ui->lineEdit_Page4Name->setText(Settings.value("4").toString());
    ui->lineEdit_Page5Name->setText(Settings.value("5").toString());
    ui->lineEdit_Page6Name->setText(Settings.value("6").toString());
    ui->lineEdit_Page7Name->setText(Settings.value("7").toString());
    ui->lineEdit_Page8Name->setText(Settings.value("8").toString());
    ui->lineEdit_Page9Name->setText(Settings.value("9").toString());
    Settings.endGroup();
    Settings.disconnect();
}

void configure_intfc::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}

void configure_intfc::SetCurrentPaqe(QString Page)
{
    CurrentPage = Page;
}

void configure_intfc::LastSoundCommand(QString Command)
{
    ui->label_command->setText(Command);
}

void configure_intfc::on_pushButton_TestInfoSound_clicked()
{
    Announce(1, "Test", "Test Info Sound");
}

void configure_intfc::on_pushButton_TestAlarmSound_clicked()
{
    Announce(2, "Test", "Test Alarm Sound");
}

void configure_intfc::on_pushButton_TestCatastrophySound_clicked()
{
    Announce(3, "Test", "Test Catastrophy Sound");
}

void configure_intfc::on_pushButton_TestLostSound_clicked()
{
    Announce(4, "Test", "Test Processor Lost Sound");
}
