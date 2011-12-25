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

#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include <QtCore>
#include <QtSql>

#include "globals.h"

class Logger : public QThread
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    qint64 SinceLast, MinTime, MaxTime, SaveTime;
    bool LogChanges;

public slots:
    void run();

private slots:
    QString Timestamp();
    void LoadConfig();
    void Purge();
};

#endif // LOGGER_H
