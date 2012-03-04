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

#ifndef PAGE_7_H
#define PAGE_7_H

#include <QWidget>

#include "globals.h"
#include "registerbyname.h"
#include "coilbyname.h"

namespace Ui {
    class Page_7;
}

class Page_7 : public QWidget
{
    Q_OBJECT

public:
    explicit Page_7(QWidget *parent = 0);
    ~Page_7();
    QString Page;

private:
    Ui::Page_7 *ui;

public slots:
    void Refresh();
    void SetPage(QString Page) {this->Page = Page;}

signals:
    void PageStatus(QString Status);

};

#endif // PAGE_7_H
