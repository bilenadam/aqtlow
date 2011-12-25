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

#include "valuelog.h"
#include "ui_valuelog.h"

#include "globals.h"
#include "registerbyname.h"
#include "coilbyname.h"

#include <QDesktopServices>
#include <QPainter>
#include <QPrinter>

ValueLog::ValueLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ValueLog)
{
    ui->setupUi(this);

    ui->comboBox_Color1->addItem("None");
    ui->comboBox_Color2->addItem("None");
    ui->comboBox_Color3->addItem("None");
    ui->comboBox_Color4->addItem("None");
    ui->dateTimeEdit_From->setDisplayFormat(TIMESTAMP_FORMAT);
    ui->dateTimeEdit_To->setDisplayFormat(TIMESTAMP_FORMAT);
    ui->dateTimeEdit_From->setDateTime(QDateTime::currentDateTime().addSecs(- 86400));
    ui->dateTimeEdit_To->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_Crosshair->setDateTime(QDateTime::currentDateTime().addSecs(- 43200));
    ui->comboBox_Page->addItem("All");
    for(int i = 0; i <= 9; i++)
    {
        ui->comboBox_Page->addItem(PageNames[i]);
    }
    LoadList();
}

ValueLog::~ValueLog()
{
    delete ui;
}

void ValueLog::on_pushButton_Export_clicked()
{
    CoilByName Cbn;
    RegisterByName Rbn;
    QString Desktop = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    QFile ExportFile(Desktop + "/Values.csv");
    ExportFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ExportStream(&ExportFile);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery Query(db);
    if(QueryString.length() > 0) Query.exec(QueryString);
    if(!Query.isActive() && ExportFile.isWritable())
    {
        ui->listWidget->clear();
        ui->listWidget->addItem("Failed to open log");
    }
    else
    {
        ExportStream << "Date/Time, Page, Name, Value, Description\n";
        while(Query.next())
        {
            ExportStream << Query.value(0).toString() + ", ";
            ExportStream << Query.value(1).toString() + ", ";
            ExportStream << Query.value(2).toString() + ", ";
            ExportStream << Query.value(3).toString() + ", ";
            if(Rbn.Found(Query.value(2).toString()))
            {
                ExportStream << Rbn.R->Description + "\n";
            }
            else
            {
                if(Cbn.Found(Query.value(2).toString()))
                {
                    ExportStream << Cbn.C->Description + "\n";
                }
            }
        }
    }
    qDebug() << "Exported " + ExportFile.fileName();
}

void ValueLog::LoadList()
{
    CoilByName Cbn;
    RegisterByName Rbn;
    QSqlDatabase db = QSqlDatabase::database();
    QString TextLine, SelectStatement, TimeFilter, PageFilter, NameFilter;
    QSqlQuery Query(db);
    SelectStatement = "SELECT * FROM Vals";
    TimeFilter = "Timestamp >= '" + ui->dateTimeEdit_From->text() + "' AND Timestamp <= '" + ui->dateTimeEdit_To->text() + "'";
    if(ui->comboBox_Page->currentIndex() > 0)
    {
        PageFilter = "Page = '" + ui->comboBox_Page->currentText() + "'";
    }
    if(ui->lineEdit_Name->text().length()>0)
    {
        NameFilter = "Name LIKE '%" + ui->lineEdit_Name->text() + "%'";
    }
    QueryString = SelectStatement + " WHERE " + TimeFilter;
    if(PageFilter.length() > 0)
    {
        QueryString = QueryString + " AND " + PageFilter;
    }
    if(NameFilter.length() > 0)
    {
        QueryString = QueryString + " AND " + NameFilter;
    }
    if(QueryString.length() > 0) Query.exec(QueryString);
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
        TextLine = TextLine.leftJustified(56, ' ');
        TextLine = TextLine + "Value";
        TextLine = TextLine.leftJustified(66, ' ');
        TextLine = TextLine + "Description";
        TextLine = TextLine.leftJustified(99, ' ');
        ui->listWidget->addItem(TextLine);
        while(Query.next())
        {
            TextLine = Query.value(0).toString();
            TextLine.truncate(25);
            TextLine = TextLine.leftJustified(26, ' ');
            TextLine = TextLine + Query.value(1).toString();
            TextLine.truncate(45);
            TextLine = TextLine.leftJustified(46, ' ');
            TextLine = TextLine + Query.value(2).toString();
            TextLine.truncate(55);
            TextLine = TextLine.leftJustified(56, ' ');
            TextLine = TextLine + Query.value(3).toString();
            TextLine.truncate(65);
            TextLine = TextLine.leftJustified(66, ' ');
            if(Rbn.Found(Query.value(2).toString()))
            {
                TextLine = TextLine + Rbn.R->Description;
            }
            else
            {
                if(Cbn.Found(Query.value(2).toString()))
                {
                    TextLine = TextLine + Cbn.C->Description;
                }
            }
            TextLine = TextLine.leftJustified(99, ' ');
            ui->listWidget->addItem(TextLine);
            if(ui->comboBox_Color1->findText(Query.value(2).toString())<0)
            {
                ui->comboBox_Color1->addItem(Query.value(2).toString());
            }
            if(ui->comboBox_Color2->findText(Query.value(2).toString())<0)
            {
                ui->comboBox_Color2->addItem(Query.value(2).toString());
            }
            if(ui->comboBox_Color3->findText(Query.value(2).toString())<0)
            {
                ui->comboBox_Color3->addItem(Query.value(2).toString());
            }
            if(ui->comboBox_Color4->findText(Query.value(2).toString())<0)
            {
                ui->comboBox_Color4->addItem(Query.value(2).toString());
            }
        }
    }
}

void ValueLog::DrawChart()
{
    CoilByName Cbn;
    RegisterByName Rbn;
    bool DrawPoint;
    QDateTime FromTime = ui->dateTimeEdit_From->dateTime();
    QDateTime ToTime = ui->dateTimeEdit_To->dateTime();
    QDateTime CrosshairTime = ui->dateTimeEdit_Crosshair->dateTime();
    QDateTime RecordTime;
    double TFrom, TTo, TRecord, TCrosshair, CHTimeRatio = 0, TimeRatio, ValueRatio = 0;
    int PointX, PointY, CrosshairX, CrosshairY;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery Query(db);
    if(QueryString.length() > 0) Query.exec(QueryString);
    if(Query.isActive())
    {
        QPixmap Canvas(ui->label_Canvas->width(), ui->label_Canvas->height());
        Canvas.fill(Qt::white);
        QPainter Vasnetsov(&Canvas);
        Vasnetsov.setBrush(Qt::SolidPattern);
        while(Query.next())
        {
            RecordTime = QDateTime::fromString(Query.value(0).toString(), TIMESTAMP_FORMAT);
            TRecord = RecordTime.toMSecsSinceEpoch();
            TCrosshair = CrosshairTime.toMSecsSinceEpoch();
            TFrom = FromTime.toMSecsSinceEpoch();
            TTo = ToTime.toMSecsSinceEpoch();
            TimeRatio = (TRecord - TFrom) / (TTo - TFrom);
            CHTimeRatio = (TCrosshair - TFrom) / (TTo - TFrom);
            //Look for the current record name among coils and registers.
            //If found and if selected as one of the colors then draw a point.
            DrawPoint = false;
            if(Cbn.Found(Query.value(2).toString()))
            {
                ValueRatio = Query.value(3).toDouble();
                if(Cbn.C->Name == ui->comboBox_Color1->currentText())
                {
                    Vasnetsov.setPen(Qt::black);
                    DrawPoint = true;
                }
                if(Cbn.C->Name == ui->comboBox_Color2->currentText())
                {
                    Vasnetsov.setPen(Qt::red);
                    DrawPoint = true;
                }
                if(Cbn.C->Name == ui->comboBox_Color3->currentText())
                {
                    Vasnetsov.setPen(Qt::blue);
                    DrawPoint = true;
                }
                if(Cbn.C->Name == ui->comboBox_Color4->currentText())
                {
                    Vasnetsov.setPen(Qt::green);
                    DrawPoint = true;
                }
            }
            if(Rbn.Found(Query.value(2).toString()))
            {
                if(Rbn.R->Name == ui->comboBox_Color1->currentText())
                {
                    ValueRatio = (Query.value(3).toDouble()-ui->doubleSpinBox_Color1Low->value())/(ui->doubleSpinBox_Color1High->value()-ui->doubleSpinBox_Color1Low->value());
                    Vasnetsov.setPen(Qt::black);
                    DrawPoint = true;
                }
                if(Rbn.R->Name == ui->comboBox_Color2->currentText())
                {
                    ValueRatio = (Query.value(3).toDouble()-ui->doubleSpinBox_Color2Low->value())/(ui->doubleSpinBox_Color2High->value()-ui->doubleSpinBox_Color2Low->value());
                    Vasnetsov.setPen(Qt::red);
                    DrawPoint = true;
                }
                if(Rbn.R->Name == ui->comboBox_Color3->currentText())
                {
                    ValueRatio = (Query.value(3).toDouble()-ui->doubleSpinBox_Color3Low->value())/(ui->doubleSpinBox_Color3High->value()-ui->doubleSpinBox_Color3Low->value());
                    Vasnetsov.setPen(Qt::blue);
                    DrawPoint = true;
                }
                if(Rbn.R->Name == ui->comboBox_Color4->currentText())
                {
                    ValueRatio = (Query.value(3).toDouble()-ui->doubleSpinBox_Color4Low->value())/(ui->doubleSpinBox_Color4High->value()-ui->doubleSpinBox_Color4Low->value());
                    Vasnetsov.setPen(Qt::green);
                    DrawPoint = true;
                }
            }
            if(DrawPoint)
            {
                PointX = (Canvas.width()-2) * TimeRatio;
                PointY = (Canvas.height()-2) * (1-ValueRatio);
                Vasnetsov.drawEllipse(PointX, PointY, 2, 2);
            }
        }
        //Draw the crosshair
        Vasnetsov.setPen(Qt::black);
        ValueRatio = double(ui->spinBox_Crosshair->value())/100;
        CrosshairX = (Canvas.width()-2) * CHTimeRatio;
        CrosshairY = (Canvas.height()-2) * (1-ValueRatio);
        Vasnetsov.drawLine(CrosshairX, CrosshairY-10, CrosshairX, CrosshairY+10);
        Vasnetsov.drawLine(CrosshairX-10, CrosshairY, CrosshairX+10, CrosshairY);
        Vasnetsov.end();
        ui->label_Canvas->setPixmap(Canvas);
        //Show what value corresponds to the crosshair for each color
        ui->lineEdit_Color1Crosshair->setText(QString::number((ui->doubleSpinBox_Color1High->value() - ui->doubleSpinBox_Color1Low->value()) * (ValueRatio) + ui->doubleSpinBox_Color1Low->value()));
        ui->lineEdit_Color2Crosshair->setText(QString::number((ui->doubleSpinBox_Color2High->value() - ui->doubleSpinBox_Color2Low->value()) * (ValueRatio) + ui->doubleSpinBox_Color2Low->value()));
        ui->lineEdit_Color3Crosshair->setText(QString::number((ui->doubleSpinBox_Color3High->value() - ui->doubleSpinBox_Color3Low->value()) * (ValueRatio) + ui->doubleSpinBox_Color3Low->value()));
        ui->lineEdit_Color4Crosshair->setText(QString::number((ui->doubleSpinBox_Color4High->value() - ui->doubleSpinBox_Color4Low->value()) * (ValueRatio) + ui->doubleSpinBox_Color4Low->value()));
    }
}

void ValueLog::on_comboBox_Color1_currentIndexChanged(int)
{
    QString Name = ui->comboBox_Color1->currentText();
    RegisterByName Rbn;
    CoilByName Cbn;
    if(Rbn.Found(Name))
    {
        ui->doubleSpinBox_Color1Low->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color1High->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color1Low->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color1High->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color1Low->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color1High->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color1Low->setValue(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color1High->setValue(Rbn.R->IntfcHigh);
    }
    if(Cbn.Found(Name))
    {
        ui->doubleSpinBox_Color1Low->setMinimum(0);
        ui->doubleSpinBox_Color1Low->setMaximum(0);
        ui->doubleSpinBox_Color1High->setMinimum(1);
        ui->doubleSpinBox_Color1High->setMaximum(1);
    }
    DrawChart();
}

void ValueLog::on_comboBox_Color2_currentIndexChanged(int)
{
    QString Name = ui->comboBox_Color2->currentText();
    RegisterByName Rbn;
    CoilByName Cbn;
    if(Rbn.Found(Name))
    {
        ui->doubleSpinBox_Color2Low->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color2High->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color2Low->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color2High->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color2Low->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color2High->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color2Low->setValue(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color2High->setValue(Rbn.R->IntfcHigh);
    }
    if(Cbn.Found(Name))
    {
        ui->doubleSpinBox_Color2Low->setMinimum(0);
        ui->doubleSpinBox_Color2Low->setMaximum(0);
        ui->doubleSpinBox_Color2High->setMinimum(1);
        ui->doubleSpinBox_Color2High->setMaximum(1);
    }
    DrawChart();
}

void ValueLog::on_comboBox_Color3_currentIndexChanged(int)
{
    QString Name = ui->comboBox_Color3->currentText();
    RegisterByName Rbn;
    CoilByName Cbn;
    if(Rbn.Found(Name))
    {
        ui->doubleSpinBox_Color3Low->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color3High->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color3Low->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color3High->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color3Low->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color3High->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color3Low->setValue(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color3High->setValue(Rbn.R->IntfcHigh);
    }
    if(Cbn.Found(Name))
    {
        ui->doubleSpinBox_Color3Low->setMinimum(0);
        ui->doubleSpinBox_Color3Low->setMaximum(0);
        ui->doubleSpinBox_Color3High->setMinimum(1);
        ui->doubleSpinBox_Color3High->setMaximum(1);
    }
    DrawChart();
}

void ValueLog::on_comboBox_Color4_currentIndexChanged(int)
{
    QString Name = ui->comboBox_Color4->currentText();
    RegisterByName Rbn;
    CoilByName Cbn;
    if(Rbn.Found(Name))
    {
        ui->doubleSpinBox_Color4Low->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color4High->setMinimum(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color4Low->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color4High->setMaximum(Rbn.R->IntfcHigh);
        ui->doubleSpinBox_Color4Low->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color4High->setSingleStep((Rbn.R->IntfcHigh-Rbn.R->IntfcLow)/10);
        ui->doubleSpinBox_Color4Low->setValue(Rbn.R->IntfcLow);
        ui->doubleSpinBox_Color4High->setValue(Rbn.R->IntfcHigh);
    }
    if(Cbn.Found(Name))
    {
        ui->doubleSpinBox_Color4Low->setMinimum(0);
        ui->doubleSpinBox_Color4Low->setMaximum(0);
        ui->doubleSpinBox_Color4High->setMinimum(1);
        ui->doubleSpinBox_Color4High->setMaximum(1);
    }
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color1Low_valueChanged(double Value)
{
    ui->doubleSpinBox_Color1High->setMinimum(Value+1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color1High_valueChanged(double Value)
{
    ui->doubleSpinBox_Color1Low->setMaximum(Value-1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color2Low_valueChanged(double Value)
{
    ui->doubleSpinBox_Color2High->setMinimum(Value+1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color2High_valueChanged(double Value)
{
    ui->doubleSpinBox_Color2Low->setMaximum(Value-1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color3Low_valueChanged(double Value)
{
    ui->doubleSpinBox_Color3High->setMinimum(Value+1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color3High_valueChanged(double Value)
{
    ui->doubleSpinBox_Color3Low->setMaximum(Value-1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color4Low_valueChanged(double Value)
{
    ui->doubleSpinBox_Color4High->setMinimum(Value+1);
    DrawChart();
}

void ValueLog::on_doubleSpinBox_Color4High_valueChanged(double Value)
{
    ui->doubleSpinBox_Color4Low->setMaximum(Value-1);
    DrawChart();
}

void ValueLog::on_pushButton_Print_clicked()
{
    emit Screenshot(QPixmap::grabWidget(this));
}

void ValueLog::on_comboBox_Page_currentIndexChanged(QString )
{
    LoadList();
    DrawChart();
}

void ValueLog::on_lineEdit_Name_textChanged(QString )
{
    LoadList();
    DrawChart();
}

void ValueLog::on_dateTimeEdit_From_dateTimeChanged(QDateTime date)
{
    ui->dateTimeEdit_To->setMinimumDateTime(date.addSecs(60));
    ui->dateTimeEdit_Crosshair->setMinimumDateTime(date);
    LoadList();
    DrawChart();
}

void ValueLog::on_dateTimeEdit_To_dateTimeChanged(QDateTime date)
{
    ui->dateTimeEdit_From->setMaximumDateTime(date.addSecs(-60));
    ui->dateTimeEdit_Crosshair->setMaximumDateTime(date);
    LoadList();
    DrawChart();
}

void ValueLog::on_pushButton_FromCrosshair_clicked()
{
    ui->dateTimeEdit_From->setDateTime(ui->dateTimeEdit_Crosshair->dateTime());
}

void ValueLog::on_pushButton_ToCrosshair_clicked()
{
    ui->dateTimeEdit_To->setDateTime(ui->dateTimeEdit_Crosshair->dateTime());
}

void ValueLog::on_CrosshairNow_clicked()
{
    ui->dateTimeEdit_To->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_Crosshair->setDateTime(QDateTime::currentDateTime());
}

void ValueLog::on_dateTimeEdit_Crosshair_dateTimeChanged(QDateTime )
{
    DrawChart();
}

void ValueLog::on_spinBox_Crosshair_valueChanged(int )
{
    DrawChart();
}

void ValueLog::mousePressEvent(QMouseEvent *event)
{
    double From = ui->dateTimeEdit_From->dateTime().toMSecsSinceEpoch();
    double To = ui->dateTimeEdit_To->dateTime().toMSecsSinceEpoch();
    QDateTime NewTime;

    if (event->button() == Qt::LeftButton) {
        QPoint Point = event->pos();
        int X = Point.x() - ui->label_Canvas->mapTo(this,QPoint(0,0)).x();
        int Y = Point.y() - ui->label_Canvas->mapTo(this,QPoint(0,0)).y();
        if( X >= 0 && Y >= 0 && X <= ui->label_Canvas->geometry().width() && Y <= ui->label_Canvas->geometry().height())
        {
            NewTime.setMSecsSinceEpoch( From + (To-From) * X / ui->label_Canvas->geometry().width() );
            ui->dateTimeEdit_Crosshair->setDateTime(NewTime);
            ui->spinBox_Crosshair->setValue( ui->spinBox_Crosshair->maximum() - ( ui->spinBox_Crosshair->maximum() * Y / ui->label_Canvas->geometry().height() ) );
        }
    }
}

void ValueLog::resizeEvent(QResizeEvent *event)
{
    DrawChart();
    event->ignore();
}

void ValueLog::on_pushButton_CrosshairHigh_clicked()
{
    ui->doubleSpinBox_Color1High->setValue(ui->lineEdit_Color1Crosshair->text().toFloat());
    ui->doubleSpinBox_Color2High->setValue(ui->lineEdit_Color2Crosshair->text().toFloat());
    ui->doubleSpinBox_Color3High->setValue(ui->lineEdit_Color3Crosshair->text().toFloat());
    ui->doubleSpinBox_Color4High->setValue(ui->lineEdit_Color4Crosshair->text().toFloat());
}

void ValueLog::on_pushButton_CrosshairLow_clicked()
{
    ui->doubleSpinBox_Color1Low->setValue(ui->lineEdit_Color1Crosshair->text().toFloat());
    ui->doubleSpinBox_Color2Low->setValue(ui->lineEdit_Color2Crosshair->text().toFloat());
    ui->doubleSpinBox_Color3Low->setValue(ui->lineEdit_Color3Crosshair->text().toFloat());
    ui->doubleSpinBox_Color4Low->setValue(ui->lineEdit_Color4Crosshair->text().toFloat());
}
