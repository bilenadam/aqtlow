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

#ifndef ALERTHANDLING_H
#define ALERTHANDLING_H

#include <QThread>

class AlertHandling : public QThread
{
    Q_OBJECT
public:
    explicit AlertHandling(QObject *parent = 0);

private:
    bool AnnouncerBusy;

signals:
    void Announce(int Level, QString Name, QString Text);

public slots:
    void run();
    void AnnounceAvailable();

private slots:
    void AlertScan();

};

#endif // ALERTHANDLING_H
