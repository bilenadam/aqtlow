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

#ifndef CURRENTALERTS_H
#define CURRENTALERTS_H

#include <QDialog>

namespace Ui {
    class CurrentAlerts;
}

class CurrentAlerts : public QDialog
{
    Q_OBJECT

signals:
    void Screenshot(QPixmap Screenshot);

public:
    explicit CurrentAlerts(QWidget *parent = 0);
    ~CurrentAlerts();

private:
    Ui::CurrentAlerts *ui;

private slots:
    void on_pushButton_Print_clicked();
    void BuildList();

private slots:

};

#endif // CURRENTALERTS_H
