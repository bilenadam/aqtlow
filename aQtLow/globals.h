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

#ifndef GLOBALS_H
#define GLOBALS_H

#include "prc.h"
#include "lognote.h"

#define NUMBER_OF_PRCS      4
#define NUMBER_OF_DTXFRUSBS 2
#define NUMBER_OF_DTXFRTCPS 4
#define TIMESTAMP_FORMAT "yyyy-MM-dd hh:mm:ss.zzz"

extern PRC P[NUMBER_OF_PRCS];
extern LOGNOTE LogNote;
extern bool PRCsOk;
extern int GAlertCount, GInfoCount, GAlarmCount, GCatastrophyCount;

extern QString MainPath, ConfigPath, LogPath, ExportPath, WavPath;

extern QString PageNames[10], CurrentPage;

extern bool Shutdown;

#endif // GLOBALS_H
