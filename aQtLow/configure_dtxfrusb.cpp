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

#include "configure_dtxfrusb.h"
#include "ui_configure_dtxfrusb.h"

#include <QSettings>
#include <QPrinter>
#include <QPainter>

configure_dtxfrusb::configure_dtxfrusb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configure_dtxfrusb)
{
    ui->setupUi(this);
    ui->spinBox_DTxfrUsb->setValue(0);
    ui->spinBox_DTxfrUsb->setMaximum(NUMBER_OF_DTXFRUSBS-1);
    LoadConfig();
}

configure_dtxfrusb::~configure_dtxfrusb()
{
    delete ui;
}

void configure_dtxfrusb::on_pushButton_Load_clicked()
{
    LoadConfig();
}

void configure_dtxfrusb::LoadConfig()
{
    QString IniFile = ConfigPath + "/" + ui->spinBox_DTxfrUsb->text() + ".dtxfrusb.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("Config");
    ui->lineEdit_Port->setText(Settings.value("Port").toString());
    ui->lineEdit_Delay->setText(Settings.value("Delay").toString());
    ui->checkBox_Enabled->setChecked(Settings.value("Enabled").toBool());
    ui->spinBox_Prc->setValue(Settings.value("Processor").toInt());
    Settings.endGroup();
    Settings.disconnect();
}

void configure_dtxfrusb::on_pushButton_Save_clicked()
{
    QString IniFile = ConfigPath + "/" + ui->spinBox_DTxfrUsb->text() + ".dtxfrusb.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("Config");
    Settings.setValue("Port", ui->lineEdit_Port->text());
    Settings.setValue("Delay", ui->lineEdit_Delay->text());
    Settings.setValue("Enabled", ui->checkBox_Enabled->isChecked());
    Settings.setValue("Processor", ui->spinBox_Prc->value());
    Settings.endGroup();
    Settings.disconnect();
}

void configure_dtxfrusb::on_pushButton_Clear_clicked()
{
    ui->lineEdit_Port->clear();
    ui->lineEdit_Delay->clear();
    ui->checkBox_Enabled->setChecked(false);
    ui->spinBox_Prc->setValue(0);
}

void configure_dtxfrusb::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}

void configure_dtxfrusb::on_spinBox_DTxfrUsb_valueChanged(int )
{
    LoadConfig();
}
