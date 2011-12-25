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

#include "notelog.h"
#include "ui_notelog.h"

#include "globals.h"

#include <QDesktopServices>
#include <QPrinter>
#include <QPainter>

NoteLog::NoteLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteLog)
{
    ui->setupUi(this);

    ui->dateTimeEdit_From->setDisplayFormat(TIMESTAMP_FORMAT);
    ui->dateTimeEdit_To->setDisplayFormat(TIMESTAMP_FORMAT);
    ui->dateTimeEdit_From->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->dateTimeEdit_To->setDateTime(QDateTime::currentDateTime());
    LoadList();
}

NoteLog::~NoteLog()
{
    delete ui;
}

void NoteLog::on_dateTimeEdit_From_dateTimeChanged(QDateTime date)
{
    ui->dateTimeEdit_To->setMinimumDateTime(date.addSecs(60));
    LoadList();
}

void NoteLog::on_dateTimeEdit_To_dateTimeChanged(QDateTime date)
{
    ui->dateTimeEdit_From->setMaximumDateTime(date.addSecs(-60));
    LoadList();
}

void NoteLog::on_pushButton_Export_clicked()
{
    QString Desktop = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    QFile ExportFile(Desktop + "/Notes.csv");
    ExportFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ExportStream(&ExportFile);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery Query(db);
    Query.exec(QueryString);
    if(!Query.isActive() && ExportFile.isWritable())
    {
        ui->listWidget->clear();
        ui->listWidget->addItem("Failed to open log");
        ui->listWidget->addItem(QueryString);
    }
    else
    {
        ExportStream << "Date/Time, Note\n";
        while(Query.next())
        {
            ExportStream << Query.value(0).toString() + ", ";
            ExportStream << Query.value(1).toString() + "\n";
        }
    }
    qDebug() << "Exported " + ExportFile.fileName();
}

void NoteLog::LoadList()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString TextLine, SelectStatement, TimeFilter;
    QSqlQuery Query(db);
    SelectStatement = "SELECT * FROM Notes";
    TimeFilter = "Timestamp >= '" + ui->dateTimeEdit_From->text() + "' AND Timestamp <= '" + ui->dateTimeEdit_To->text() + "'";
    QueryString = SelectStatement + " WHERE " + TimeFilter;
    Query.exec(QueryString);
    if(!Query.isActive())
    {
        ui->listWidget->clear();
        ui->listWidget->addItem("Failed to open log");
    }
    else
    {
        ui->listWidget->clear();
        TextLine = "Date/Time";
        TextLine = TextLine.leftJustified(26, ' ');
        TextLine = TextLine + "Note";
        TextLine = TextLine.leftJustified(99, ' ');
        ui->listWidget->addItem(TextLine);
        while(Query.next() and ui->listWidget->count() < 10000)
        {
            TextLine = Query.value(0).toString();
            TextLine.truncate(25);
            TextLine = TextLine.leftJustified(26, ' ');
            TextLine = TextLine + Query.value(1).toString();
            TextLine = TextLine.leftJustified(99, ' ');
            ui->listWidget->addItem(TextLine);
        }
        if(ui->listWidget->count() >= 10000)ui->listWidget->addItem("Stopped at 10000 records, you might want to filter that a little");
    }
}

void NoteLog::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}
