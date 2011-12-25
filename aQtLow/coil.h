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

#ifndef COIL_H
#define COIL_H

#include <QObject>

#include <QString>

class COIL : public QObject
{
        Q_OBJECT
public:
    explicit COIL(QObject *parent = 0);
    QString Name, Description, Short, Page;
    bool PrcValue, PrcWrite, Value;
    bool IsConfigured, FirstRead, WriteRequest;
    bool IsLogged, LastLogValue, LogAlertRequest, Announce;
    int Alert, AlertIf, LogAlertChange;
    qint64 LastLogTime;

signals:
    void Changed(void);
    void Changed(bool Value);

public slots:
    void FromPrc(bool FP);
    void Set();
    void Reset();
    void Toggle();

private:
    void CombinedWrite(bool FH);

};

#endif // COIL_H
