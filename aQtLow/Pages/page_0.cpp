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

#include "page_0.h"
#include "ui_page_0.h"

Page_0::Page_0(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_0)
{
    ui->setupUi(this);
}

Page_0::~Page_0()
{
    delete ui;
}

void Page_0::Refresh()
{    
    if(!this->isHidden()) emit PageStatus(Page);

    RegisterByName Rbn;
    CoilByName Cbn;

    int BarMin, BarMax;

    if(Rbn.Found("V", Page)) ui->widget_V->Setup(Rbn.p, Rbn.r, 0, 10, 3);
    if(Rbn.Found("VLo", Page)) ui->widget_Lo->Setup(Rbn.p, Rbn.r, 1, 10, 3);
    if(Rbn.Found("VHi", Page)) ui->widget_Hi->Setup(Rbn.p, Rbn.r, 1, 10, 3);
    if(Rbn.Found("Range", Page))
    {
        ui->widget_Range->Setup(Rbn.p, Rbn.r, 1, 10, 1);
        BarMin = 0;
        BarMax = Rbn.R->Value;
    }
    if(Rbn.Found("NL", Page)) ui->widget_NL->Setup(Rbn.p, Rbn.r, 1, 10, 3);

    if(Rbn.Found("Lbs", Page))
    {
        ui->widget_Lbs->Setup(Rbn.p, Rbn.r, 0, 30, 1);
        ui->progressBar_Lbs->setValue(Rbn.R->Value);
        ui->progressBar_Lbs->setMinimum(BarMin);
        ui->progressBar_Lbs->setMaximum(BarMax);
        ui->progressBar_Lbs->repaint();
    }

    if(Rbn.Found("MaxLbs", Page)) ui->widget_MaxLbs->Setup(Rbn.p, Rbn.r, 0, 30, 1);
    if(Rbn.Found("Score", Page)) ui->widget_Score->Setup(Rbn.p, Rbn.r, 0, 30, 1);
}
