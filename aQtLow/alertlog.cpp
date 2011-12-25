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

#include "alertlog.h"
#include "ui_alertlog.h"

#include "globals.h"
#include "coilbyname.h"

#include <QDesktopServices>
#include <QPrinter>
#include <QPainter>

AlertLog::AlertLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertLog)
{
    ui->setupUi(this);
    ui->comboBox_Level->addItem("Catastrophy Off");
    ui->comboBox_Level->addItem("Alarm Off");
    ui->comboBox_Level->addItem("Info Off");
    ui->comboBox_Level->addItem("All");
    ui->comboBox_Level->addItem("Info On");
    ui->comboBox_Level->addItem("Alarm On");
    ui->comboBox_Level->addItem("Catastrophy On");
    ui->comboBox_Level->setCurrentIndex(3);
    ui->dateTimeEdit_From->setDisplayFormat(TIMESTAMP_FORMAT);
    ui->dateTimeEdit_To->setDisplayFormat(TIMESTAMP_FORMAT);
    ui->dateTimeEdit_From->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dateTimeEdit_To->setDateTime(QDateTime::currentDateTime());
    ui->comboBox_Page->addItem("All");
    for(int i = 0; i <= 9; i++)
    {
        ui->comboBox_Page->addItem(PageNames[i]);
    }
    LoadList();
}

AlertLog::~AlertLog()
{
    delete ui;
}

void AlertLog::LoadList()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString TextLine, SelectStatement, TimeFilter, NameFilter, PageFilter, LevelFilter;
    QSqlQuery Query(db);
    SelectStatement = "SELECT * FROM Alerts";
    TimeFilter = "Timestamp >= '" + ui->dateTimeEdit_From->text() + "' AND Timestamp <= '" + ui->dateTimeEdit_To->text() + "'";
    if(ui->lineEdit_Name->text().length()>0)
    {
        NameFilter = "Name LIKE '%" + ui->lineEdit_Name->text() + "%'";
    }
    if(ui->comboBox_Level->currentIndex() != 3)
    {
        LevelFilter = "Alert = '" + QString::number(ui->comboBox_Level->currentIndex() - 3) + "'";
    }
    if(ui->comboBox_Page->currentIndex() > 0)
    {
        PageFilter = "Page = '" + ui->comboBox_Page->currentText() + "'";
    }
    QueryString = SelectStatement + " WHERE " + TimeFilter;
    if(NameFilter.length() > 0)
    {
        QueryString = QueryString + " AND " + NameFilter;
    }
    if(PageFilter.length() > 0)
    {
        QueryString = QueryString + " AND " + PageFilter;
    }
    if(LevelFilter.length() > 0)
    {
        QueryString = QueryString + " AND " + LevelFilter;
    }
    Query.exec(QueryString);
    if(!Query.isActive())
    {
        ui->listWidget->clear();
        ui->listWidget->addItem("Failed to open log");
        ui->listWidget->addItem(QueryString);
    }
    else
    {
        ui->listWidget->clear();
        TextLine = "Date/Time";
        TextLine = TextLine.leftJustified(26, ' ');
        TextLine = TextLine + "Page";
        TextLine = TextLine.leftJustified(46, ' ');
        TextLine = TextLine + "Name";
        TextLine = TextLine.leftJustified(66, ' ');
        TextLine = TextLine + "L";
        TextLine = TextLine.leftJustified(71, ' ');
        TextLine = TextLine + "Description";
        TextLine = TextLine.leftJustified(99, ' ');
        ui->listWidget->addItem(TextLine);
        while(Query.next() and ui->listWidget->count() < 10000)
        {
            TextLine = Query.value(0).toString();
            TextLine.truncate(25);
            TextLine = TextLine.leftJustified(26, ' ');
            TextLine = TextLine + Query.value(1).toString();
            TextLine.truncate(45);
            TextLine = TextLine.leftJustified(46, ' ');
            TextLine = TextLine + Query.value(2).toString();
            TextLine.truncate(65);
            TextLine = TextLine.leftJustified(66, ' ');
            TextLine = TextLine + Query.value(3).toString();
            TextLine.truncate(70);
            TextLine = TextLine.leftJustified(71, ' ');
            TextLine = TextLine + Query.value(4).toString();
            TextLine = TextLine.leftJustified(99, ' ');
            ui->listWidget->addItem(TextLine);
        }
        if(ui->listWidget->count() >= 10000)ui->listWidget->addItem("Stopped at 10000 records, you might want to filter that a little");
    }
}

void AlertLog::on_comboBox_Page_currentIndexChanged(QString )
{
    LoadList();
}

void AlertLog::on_comboBox_Level_currentIndexChanged(QString )
{
    LoadList();
}

void AlertLog::on_lineEdit_Name_textChanged(QString )
{
    LoadList();
}

void AlertLog::on_dateTimeEdit_To_dateTimeChanged(QDateTime date)
{
    ui->dateTimeEdit_From->setMaximumDateTime(date.addSecs(-60));
    LoadList();
}

void AlertLog::on_dateTimeEdit_From_dateTimeChanged(QDateTime date)
{
    ui->dateTimeEdit_To->setMinimumDateTime(date.addSecs(60));
    LoadList();
}
void AlertLog::on_pushButton_Export_clicked()
{
    QString Desktop = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    QFile ExportFile(Desktop + "/Alerts.csv");
    ExportFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ExportStream(&ExportFile);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery Query(db);
    Query.exec(QueryString);
    if(!Query.isActive() && ExportFile.isWritable())
    {
        ui->listWidget->clear();
        ui->listWidget->addItem("Failed to open log");
    }
    else
    {
        ExportStream << "Date/Time, Page, Name, L, Description\n";
        while(Query.next())
        {
            ExportStream << Query.value(0).toString() + ", ";
            ExportStream << Query.value(1).toString() + ", ";
            ExportStream << Query.value(2).toString() + ", ";
            ExportStream << Query.value(3).toString() + ", ";
            ExportStream << Query.value(4).toString() + "\n";
        }
    }
    qDebug() << "Exported " + ExportFile.fileName();
}

void AlertLog::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}
