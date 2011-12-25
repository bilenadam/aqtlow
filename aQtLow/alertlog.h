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

#ifndef ALERTLOG_H
#define ALERTLOG_H

#include <QDialog>

#include <QtSql>

namespace Ui {
    class AlertLog;
}

class AlertLog : public QDialog
{
    Q_OBJECT

signals:
    void Screenshot(QPixmap Screenshot);

public:
    explicit AlertLog(QWidget *parent = 0);
    ~AlertLog();

private:
    Ui::AlertLog *ui;
    QString QueryString;

private slots:
    void LoadList();
    void on_pushButton_Print_clicked();
    void on_pushButton_Export_clicked();
    void on_dateTimeEdit_From_dateTimeChanged(QDateTime date);
    void on_dateTimeEdit_To_dateTimeChanged(QDateTime date);
    void on_lineEdit_Name_textChanged(QString );
    void on_comboBox_Level_currentIndexChanged(QString );
    void on_comboBox_Page_currentIndexChanged(QString );
};

#endif // ALERTLOG_H
