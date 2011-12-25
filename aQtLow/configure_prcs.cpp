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

#include "configure_prcs.h"
#include "ui_configure_prcs.h"

#include <QSettings>
#include <QTimer>
#include <QPrinter>
#include <QPainter>

Configure_PRCs::Configure_PRCs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configure_PRCs)
{
    ui->setupUi(this);

    ui->spinBox_PRC->setValue(0);
    ui->spinBox_PRC->setMaximum(NUMBER_OF_PRCS-1);
    ui->spinBox_Register->setValue(0);
    ui->spinBox_Register->setMaximum(NUMBER_OF_REGISTERS-1);
    ui->spinBox_Coil->setValue(0);
    ui->spinBox_Coil->setMaximum(NUMBER_OF_COILS-1);
    LoadConfig();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(CurrentValues()));
    timer->start(100);

    for(int i = 0; i <= 9; i++)
    {
        ui->comboBox_RPage->addItem(PageNames[i]);
        ui->comboBox_CPage->addItem(PageNames[i]);
    }
}

Configure_PRCs::~Configure_PRCs()
{
    delete ui;
}

void Configure_PRCs::CurrentValues()
{
    int p = ui->spinBox_PRC->text().toInt();
    int r = ui->spinBox_Register->text().toInt();
    ui->lineEdit_RIntfcValue->setText(QString::number(P[p].R[r].Value));
    ui->lineEdit_RPrcValue->setText(QString::number(P[p].R[r].PrcValue));
    int c = ui->spinBox_Coil->text().toInt();
    ui->lineEdit_CIntfcValue->setText(QString::number(P[p].C[c].Value));
    ui->lineEdit_CAlert->setText(QString::number(P[p].C[c].Alert));    
    if(P[p].CommOk)
    {
        ui->groupBox_CurrentValues->setTitle("Current Values - Comm OK");
    }
    else
    {
        ui->groupBox_CurrentValues->setTitle("No Communication");
        ui->lineEdit_RIntfcValue->setText("");
        ui->lineEdit_RPrcValue->setText("");
        ui->lineEdit_CIntfcValue->setText("");
        ui->lineEdit_CAlert->setText("");
    }
}

void Configure_PRCs::on_pushButton_Load_clicked()
{
    LoadConfig();
}

void Configure_PRCs::on_pushButton_Save_clicked()
{
    QString IniFile = ConfigPath + "/" + ui->spinBox_PRC->text() + ".prc.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    switch(ui->tabWidget->currentIndex())
    {
    case 0: //Register tab
        Settings.beginGroup("R"+ui->spinBox_Register->text());
        Settings.setValue("Name", ui->lineEdit_RName->text());
        Settings.setValue("Description", ui->lineEdit_RDescription->text());
        Settings.setValue("Units", ui->lineEdit_RUnits->text());
        Settings.setValue("Page", ui->comboBox_RPage->currentText());
        Settings.setValue("IsLogged", QString::number(ui->comboBox_RIsLogged->currentIndex()));
        Settings.setValue("LogFilter", ui->lineEdit_RLogFilter->text());
        Settings.setValue("PrcLow", ui->lineEdit_RPrcLow->text());
        Settings.setValue("PrcHigh", ui->lineEdit_RPrcHigh->text());
        Settings.setValue("IntfcLow", ui->lineEdit_RIntfcLow->text());
        Settings.setValue("IntfcHigh", ui->lineEdit_RIntfcHigh->text());
        Settings.setValue("IntfcMin", ui->lineEdit_RIntfcMin->text());
        Settings.setValue("IntfcMax", ui->lineEdit_RIntfcMax->text());
        Settings.endGroup();
        break;
    case 1: //Coil tab
        Settings.beginGroup("C"+ui->spinBox_Coil->text());
        Settings.setValue("Name", ui->lineEdit_CName->text());
        Settings.setValue("Description", ui->lineEdit_CDescription->text());
        Settings.setValue("Page", ui->comboBox_CPage->currentText());
        Settings.setValue("IsLogged", QString::number(ui->comboBox_CIsLogged->currentIndex()));
        Settings.setValue("AlertIf", QString::number(ui->comboBox_CAlert->currentIndex()-3));
        Settings.endGroup();
        break;
    }
    Settings.disconnect();
}

void Configure_PRCs::on_pushButton_Copy_clicked()
{
    switch(ui->tabWidget->currentIndex())
    {
    case 0: //Register tab
        Clip[0] = ui->lineEdit_RName->text();
        Clip[1] = ui->lineEdit_RDescription->text();
        Clip[2] = ui->lineEdit_RUnits->text();
        Clip[3] = ui->comboBox_RPage->currentText();
        Clip[4] = QString::number(ui->comboBox_RIsLogged->currentIndex());
        Clip[5] = ui->lineEdit_RLogFilter->text();
        Clip[6] = ui->lineEdit_RPrcLow->text();
        Clip[7] = ui->lineEdit_RPrcHigh->text();
        Clip[8] = ui->lineEdit_RIntfcLow->text();
        Clip[9] = ui->lineEdit_RIntfcHigh->text();
        Clip[10] = ui->lineEdit_RIntfcMin->text();
        Clip[11] = ui->lineEdit_RIntfcMax->text();
        break;
    case 1: //Coil tab
        Clip[0] = ui->lineEdit_CName->text();
        Clip[1] = ui->lineEdit_CDescription->text();
        Clip[2] = ui->comboBox_CPage->currentText();
        Clip[3] = QString::number(ui->comboBox_CIsLogged->currentIndex());
        Clip[4] = QString::number(ui->comboBox_CAlert->currentIndex());
        break;
    }
}

void Configure_PRCs::on_pushButton_Paste_clicked()
{
    switch(ui->tabWidget->currentIndex())
    {
    case 0: //Register tab
        ui->lineEdit_RName->setText(Clip[0]);
        ui->lineEdit_RDescription->setText(Clip[1]);
        ui->lineEdit_RUnits->setText(Clip[2]);
        ui->comboBox_RPage->setCurrentIndex(ui->comboBox_RPage->findText(Clip[3]));
        ui->comboBox_RIsLogged->setCurrentIndex(Clip[4].toInt());
        ui->lineEdit_RLogFilter->setText(Clip[5]);
        ui->lineEdit_RPrcLow->setText(Clip[6]);
        ui->lineEdit_RPrcHigh->setText(Clip[7]);
        ui->lineEdit_RIntfcLow->setText(Clip[8]);
        ui->lineEdit_RIntfcHigh->setText(Clip[9]);
        ui->lineEdit_RIntfcMin->setText(Clip[10]);
        ui->lineEdit_RIntfcMax->setText(Clip[11]);
        break;
    case 1: //Coil tab
        ui->lineEdit_CName->setText(Clip[0]);
        ui->lineEdit_CDescription->setText(Clip[1]);
        ui->comboBox_CPage->setCurrentIndex(ui->comboBox_CPage->findText(Clip[2]));
        ui->comboBox_CIsLogged->setCurrentIndex(Clip[3].toInt());
        ui->comboBox_CAlert->setCurrentIndex(Clip[4].toInt());
        break;
    }
}

void Configure_PRCs::on_pushButton_Clear_clicked()
{
    switch(ui->tabWidget->currentIndex())
    {
    case 0: //Register tab
        ui->lineEdit_RName->clear();
        ui->lineEdit_RDescription->clear();
        ui->lineEdit_RUnits->clear();
        ui->comboBox_RPage->setCurrentIndex(-1);
        ui->comboBox_RIsLogged->setCurrentIndex(0);
        ui->lineEdit_RLogFilter->clear();
        ui->lineEdit_RPrcLow->clear();
        ui->lineEdit_RPrcHigh->clear();
        ui->lineEdit_RIntfcLow->clear();
        ui->lineEdit_RIntfcHigh->clear();
        ui->lineEdit_RIntfcMin->clear();
        ui->lineEdit_RIntfcMax->clear();
        break;
    case 1: //Coil tab
        ui->lineEdit_CName->clear();
        ui->lineEdit_CDescription->clear();
        ui->comboBox_CPage->setCurrentIndex(-1);
        ui->comboBox_CIsLogged->setCurrentIndex(0);
        ui->comboBox_CAlert->setCurrentIndex(3);
        break;
    }
}

void Configure_PRCs::LoadConfig()
{
    QString IniFile = ConfigPath + "/" + ui->spinBox_PRC->text() + ".prc.ini";
    QSettings Settings(IniFile, QSettings::IniFormat);
    Settings.beginGroup("R"+ui->spinBox_Register->text());
    ui->lineEdit_RName->setText(Settings.value("Name").toString());
    ui->lineEdit_RDescription->setText(Settings.value("Description").toString());
    ui->lineEdit_RUnits->setText(Settings.value("Units").toString());
    ui->comboBox_RPage->setCurrentIndex(ui->comboBox_RPage->findText(Settings.value("Page").toString()));
    ui->comboBox_RIsLogged->setCurrentIndex(Settings.value("IsLogged").toInt());
    ui->lineEdit_RLogFilter->setText(Settings.value("LogFilter").toString());
    ui->lineEdit_RPrcLow->setText(Settings.value("PrcLow").toString());
    ui->lineEdit_RPrcHigh->setText(Settings.value("PrcHigh").toString());
    ui->lineEdit_RIntfcLow->setText(Settings.value("IntfcLow").toString());
    ui->lineEdit_RIntfcHigh->setText(Settings.value("IntfcHigh").toString());
    ui->lineEdit_RIntfcMin->setText(Settings.value("IntfcMin").toString());
    ui->lineEdit_RIntfcMax->setText(Settings.value("IntfcMax").toString());
    Settings.endGroup();
    Settings.beginGroup("C"+ui->spinBox_Coil->text());
    ui->lineEdit_CName->setText(Settings.value("Name").toString());
    ui->lineEdit_CDescription->setText(Settings.value("Description").toString());
    ui->comboBox_CPage->setCurrentIndex(ui->comboBox_CPage->findText(Settings.value("Page").toString()));
    ui->comboBox_CIsLogged->setCurrentIndex(Settings.value("IsLogged").toInt());
    ui->comboBox_CAlert->setCurrentIndex(Settings.value("AlertIf","0").toInt()+3);
    Settings.endGroup();
    Settings.disconnect();
}

void Configure_PRCs::on_spinBox_PRC_valueChanged(int )
{
    LoadConfig();
}

void Configure_PRCs::on_spinBox_Coil_valueChanged(int )
{
    LoadConfig();
}

void Configure_PRCs::on_spinBox_Register_valueChanged(int )
{
    LoadConfig();
}


void Configure_PRCs::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}

