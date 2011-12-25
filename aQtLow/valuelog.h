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

#ifndef VALUELOG_H
#define VALUELOG_H

#include <QDialog>

#include <QtSql>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
    class ValueLog;
}

class ValueLog : public QDialog
{
    Q_OBJECT

public:
    explicit ValueLog(QWidget *parent = 0);
    ~ValueLog();

signals:
    void Screenshot(QPixmap Screenshot);

public slots:
    void LoadList();
    void DrawChart();

private slots:
    void on_CrosshairNow_clicked();
    void on_pushButton_ToCrosshair_clicked();
    void on_pushButton_FromCrosshair_clicked();
    void on_comboBox_Page_currentIndexChanged(QString );
    void on_dateTimeEdit_Crosshair_dateTimeChanged(QDateTime date);
    void on_pushButton_Print_clicked();
    void on_doubleSpinBox_Color4High_valueChanged(double );
    void on_doubleSpinBox_Color4Low_valueChanged(double );
    void on_doubleSpinBox_Color3High_valueChanged(double );
    void on_doubleSpinBox_Color3Low_valueChanged(double );
    void on_doubleSpinBox_Color2High_valueChanged(double );
    void on_doubleSpinBox_Color2Low_valueChanged(double );
    void on_doubleSpinBox_Color1High_valueChanged(double );
    void on_doubleSpinBox_Color1Low_valueChanged(double );
    void on_comboBox_Color4_currentIndexChanged(int );
    void on_comboBox_Color3_currentIndexChanged(int );
    void on_comboBox_Color2_currentIndexChanged(int );
    void on_comboBox_Color1_currentIndexChanged(int );
    void on_pushButton_Export_clicked();
    void on_dateTimeEdit_From_dateTimeChanged(QDateTime date);
    void on_dateTimeEdit_To_dateTimeChanged(QDateTime date);
    void on_lineEdit_Name_textChanged(QString );
    void on_spinBox_Crosshair_valueChanged(int );
    void on_pushButton_CrosshairHigh_clicked();
    void on_pushButton_CrosshairLow_clicked();

private:
    Ui::ValueLog *ui;
    QString QueryString;
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

};

#endif // VALUELOG_H
