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

#ifndef CONFIGURE_DTXFRTCP_H
#define CONFIGURE_DTXFRTCP_H

#include <QDialog>

#include "globals.h"

namespace Ui {
    class configure_dtxfrtcp;
}

class configure_dtxfrtcp : public QDialog
{
    Q_OBJECT

signals:
    void Screenshot(QPixmap Screenshot);

public:
    explicit configure_dtxfrtcp(QWidget *parent = 0);
    ~configure_dtxfrtcp();

private:
    Ui::configure_dtxfrtcp *ui;

private slots:
    void on_pushButton_Print_clicked();
    void on_pushButton_Save_clicked();
    void on_pushButton_Load_clicked();
    void LoadConfig();
    void on_pushButton_Clear_clicked();
    void on_spinBox_DTxfrTcp_valueChanged(int );

};

#endif // CONFIGURE_DTXFRTCP_H
