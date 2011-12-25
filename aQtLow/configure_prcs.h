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

#ifndef CONFIGURE_PRCS_H
#define CONFIGURE_PRCS_H

#include <QDialog>

#include <QString>

#include "globals.h"

namespace Ui {
    class Configure_PRCs;
}

class Configure_PRCs : public QDialog
{
    Q_OBJECT

signals:
    void Screenshot(QPixmap Screenshot);

public:
    explicit Configure_PRCs(QWidget *parent = 0);
    ~Configure_PRCs();
    QString Clip[32];

private:
    Ui::Configure_PRCs *ui;

private slots:
    void on_pushButton_Print_clicked();
    void on_spinBox_Register_valueChanged(int );
    void on_spinBox_Coil_valueChanged(int );
    void on_spinBox_PRC_valueChanged(int );
    void LoadConfig();    
    void CurrentValues();
    void on_pushButton_Paste_clicked();
    void on_pushButton_Copy_clicked();
    void on_pushButton_Save_clicked();
    void on_pushButton_Load_clicked();
    void on_pushButton_Clear_clicked();
};

#endif // CONFIGURE_PRCS_H
