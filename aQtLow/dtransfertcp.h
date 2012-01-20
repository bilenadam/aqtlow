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

#ifndef DTRANSFERTCP_H
#define DTRANSFERTCP_H

#include <QThread>
#include <QtCore>
#include <QTcpSocket>

struct TcpTransferConfiguration {
    QString Address;
    int Prc, DTxfrTcp, Delay;
};

class dtransfertcp : public QThread
{
    Q_OBJECT
public:
    explicit dtransfertcp(QObject *parent = 0);

private:
    QTcpSocket *Socket;
    TcpTransferConfiguration Cfg;
    int ExpectingResponse;

signals:
    void Refresh();

public slots:
    void run();
    void Init(QString Path, int DTxfrTcpNum);

private slots:
    void Send(int Function, int Position, int Data);
    void Receive();
    int  WriteRequest();

};

#endif // DTRANSFERTCP_H
