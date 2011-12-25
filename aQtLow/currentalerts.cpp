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

#include "currentalerts.h"
#include "ui_currentalerts.h"

#include "globals.h"

#include <QTimer>
#include <QSettings>
#include <QPrinter>
#include <QPainter>

CurrentAlerts::CurrentAlerts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentAlerts)
{
    ui->setupUi(this);

    ui->comboBox_Level->addItem("All");
    ui->comboBox_Level->addItem("Info");
    ui->comboBox_Level->addItem("Alarm");
    ui->comboBox_Level->addItem("Catastrophy");
    ui->comboBox_Level->setCurrentIndex(0);
    ui->comboBox_Page->addItem("All");
    for(int i = 0; i <= 9; i++)
    {
        ui->comboBox_Page->addItem(PageNames[i]);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(BuildList()));
    timer->start(1000);
}

CurrentAlerts::~CurrentAlerts()
{
    delete ui;
}

void CurrentAlerts::BuildList()
{
    QString TextLine;
    int Position = ui->listWidget->currentRow();
    ui->listWidget->clear();
    TextLine = "Name";
    TextLine = TextLine.leftJustified(20, ' ');
    TextLine = TextLine + "L";
    TextLine = TextLine.leftJustified(25, ' ');
    TextLine = TextLine + "Page";
    TextLine = TextLine.leftJustified(45, ' ');
    TextLine = TextLine + "Description";
    TextLine = TextLine.leftJustified(99, ' ');
    ui->listWidget->addItem(TextLine);
    for(int p = 0; p < NUMBER_OF_PRCS; p++)
    {
        if(P[p].Active)
        {            
            for(int c = 0; c < NUMBER_OF_COILS; c++)
            {
                TextLine = P[p].C[c].Name;
                TextLine.truncate(19);
                TextLine = TextLine.leftJustified(20, ' ');
                TextLine = TextLine + QString::number(P[p].C[c].Alert);
                TextLine.truncate(24);
                TextLine = TextLine.leftJustified(25, ' ');
                TextLine = TextLine + P[p].C[c].Page;
                TextLine.truncate(44);
                TextLine = TextLine.leftJustified(45, ' ');
                TextLine = TextLine + P[p].C[c].Description;
                TextLine = TextLine.leftJustified(99, ' ');
                if(P[p].C[c].Alert > 0)
                {
                    if(ui->comboBox_Page->currentText()=="All" || ui->comboBox_Page->currentText()==P[p].C[c].Page)
                    {
                        if(ui->comboBox_Level->currentText()=="All" || ui->comboBox_Level->currentIndex()==P[p].C[c].Alert)
                        {
                            ui->listWidget->addItem(TextLine);
                        }
                    }
                }
            }
        }
    }
    if(Position >= ui->listWidget->count()) Position = ui->listWidget->count() - 1;
    ui->listWidget->setCurrentRow(Position);
}

void CurrentAlerts::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}
