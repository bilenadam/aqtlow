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

#ifndef CONFIGURE_INTFC_H
#define CONFIGURE_INTFC_H

#include <QDialog>

namespace Ui {
    class configure_intfc;
}

class configure_intfc : public QDialog
{
    Q_OBJECT

signals:
    void Screenshot(QPixmap Screenshot);
    void Announce(int Level, QString Name, QString Text);

public:
    explicit configure_intfc(QWidget *parent = 0);
    ~configure_intfc();

private:
    Ui::configure_intfc *ui;
    QString CurrentPage;

public slots:
    void SetCurrentPaqe(QString Page);
    void LastSoundCommand(QString Command);

private slots:
    void on_pushButton_Print_clicked();
    void on_pushButton_Save_clicked();
    void LoadConfig();
    void on_pushButton_TestInfoSound_clicked();
    void on_pushButton_TestAlarmSound_clicked();
    void on_pushButton_TestCatastrophySound_clicked();
    void on_pushButton_TestLostSound_clicked();
};

#endif // CONFIGURE_INTFC_H
