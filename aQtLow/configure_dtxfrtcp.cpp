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

#include "configure_dtxfrtcp.h"
#include "ui_configure_dtxfrtcp.h"

#include <QSettings>
#include <QPrinter>
#include <QPainter>

configure_dtxfrtcp::configure_dtxfrtcp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configure_dtxfrtcp)
{
    ui->setupUi(this);
    ui->spinBox_DTxfrTcp->setValue(0);
    ui->spinBox_DTxfrTcp->setMaximum(NUMBER_OF_DTXFRTCPS-1);
    LoadConfig();
}

configure_dtxfrtcp::~configure_dtxfrtcp()
{
    delete ui;
}

void configure_dtxfrtcp::on_pushButton_Load_clicked()
{
    LoadConfig();
}

void configure_dtxfrtcp::LoadConfig()
{
    QString IniFile = ConfigPath + "/" + ui->spinBox_DTxfrTcp->text() + ".dtxfrtcp.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("Config");
    ui->lineEdit_Address->setText(Settings.value("Address").toString());
    ui->lineEdit_Delay->setText(Settings.value("Delay").toString());
    ui->checkBox_Enabled->setChecked(Settings.value("Enabled").toBool());
    ui->spinBox_Prc->setValue(Settings.value("Processor").toInt());
    Settings.endGroup();
    Settings.disconnect();
}

void configure_dtxfrtcp::on_pushButton_Save_clicked()
{
    QString IniFile = ConfigPath + "/" + ui->spinBox_DTxfrTcp->text() + ".dtxfrtcp.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("Config");
    Settings.setValue("Address", ui->lineEdit_Address->text());
    Settings.setValue("Delay", ui->lineEdit_Delay->text());
    Settings.setValue("Enabled", ui->checkBox_Enabled->isChecked());
    Settings.setValue("Processor", ui->spinBox_Prc->value());
    Settings.endGroup();
    Settings.disconnect();
}

void configure_dtxfrtcp::on_pushButton_Clear_clicked()
{
    ui->lineEdit_Address->clear();
    ui->lineEdit_Delay->clear();
    ui->checkBox_Enabled->setChecked(false);
    ui->spinBox_Prc->setValue(0);
}

void configure_dtxfrtcp::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}

void configure_dtxfrtcp::on_spinBox_DTxfrTcp_valueChanged(int )
{
    LoadConfig();
}
