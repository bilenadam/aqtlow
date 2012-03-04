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

#include "page_9.h"
#include "ui_page_9.h"

Page_9::Page_9(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_9)
{
    ui->setupUi(this);
}

Page_9::~Page_9()
{
    delete ui;
}

void Page_9::Refresh()
{
    if(!this->isHidden()) emit PageStatus(Page);

}
