#    aQtLow - an Arduino interface.
#    Copyright (C) 2011  Dee Wykoff
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#-------------------------------------------------
#
# Project created by QtCreator 2011-10-22T15:53:45
#
#-------------------------------------------------

QT += core gui

TARGET = aQtLow
TEMPLATE = app

# for sockets
QT += network

# for Sqlite
QT += sql

# Link to QExtSerialPort per their instructions.
# This "pri" method became available with version 1.2Beta2
# This should be the only mention of qextserailport in this pro file.
include(qextserialport/src/qextserialport.pri)

INCLUDEPATH += Pages        # Page meant to be used as "promote tos"
INCLUDEPATH += Widgets      # Custom widgets

SOURCES += main.cpp\
    mainwindow.cpp \
    register.cpp \
    coil.cpp \
    prc.cpp \
    configure_prcs.cpp \
    globals.cpp \
    logger.cpp \
    configure_intfc.cpp \
    alerthandling.cpp \
    currentalerts.cpp \
    registerbyname.cpp \
    coilbyname.cpp \
    alertlog.cpp \
    valuelog.cpp \
    Pages/page_0.cpp \
    Widgets/registerdisplay.cpp \
    Widgets/registerbutton.cpp \
    Widgets/coildisplay.cpp \
    Widgets/coilbutton.cpp \
    Pages/page_1.cpp \
    Pages/page_2.cpp \
    Pages/page_3.cpp \
    Pages/page_4.cpp \
    Pages/page_5.cpp \
    Pages/page_6.cpp \
    Pages/page_7.cpp \
    Pages/page_8.cpp \
    Pages/page_9.cpp \
    notelog.cpp \
    lognote.cpp \
    configure_dtxfrusb.cpp \
    dtransferusb.cpp \
    about.cpp \
    configure_dtxfrtcp.cpp \
    dtransfertcp.cpp

HEADERS += mainwindow.h \
    register.h \
    coil.h \
    prc.h \
    configure_prcs.h \
    globals.h \
    logger.h \
    configure_intfc.h \
    alerthandling.h \
    currentalerts.h \
    registerbyname.h \
    coilbyname.h \
    alertlog.h \
    valuelog.h \
    Pages/page_0.h \
    Widgets/registerdisplay.h \
    Widgets/registerbutton.h \
    Widgets/coildisplay.h \
    Widgets/coilbutton.h \
    Pages/page_1.h \
    Pages/page_2.h \
    Pages/page_3.h \
    Pages/page_4.h \
    Pages/page_5.h \
    Pages/page_6.h \
    Pages/page_7.h \
    Pages/page_8.h \
    Pages/page_9.h \
    configure_prcs.h \
    notelog.h \
    lognote.h \
    dtransferusb.h \
    configure_dtxfrusb.h \
    configure_dtxfrusb.h \
    dtransferusb.h \
    about.h \
    configure_dtxfrtcp.h \
    dtransfertcp.h

FORMS += mainwindow.ui \
    configure_prcs.ui \
    configure_intfc.ui \
    currentalerts.ui \
    alertlog.ui \
    valuelog.ui \
    Pages/page_0.ui \
    Widgets/registerbutton.ui \
    Widgets/coildisplay.ui \
    Widgets/coilbutton.ui \
    Widgets/registerdisplay.ui \
    Pages/page_1.ui \
    Pages/page_2.ui \
    Pages/page_3.ui \
    Pages/page_4.ui \
    Pages/page_5.ui \
    Pages/page_6.ui \
    Pages/page_7.ui \
    Pages/page_8.ui \
    Pages/page_9.ui \
    notelog.ui \
    configure_dtxfrusb.ui \
    about.ui \
    configure_dtxfrtcp.ui

wince*: DEPLOYMENT_PLUGIN += qsqlite

RESOURCES += \
    Resources.qrc

OTHER_FILES += \
    Notes.txt \
    Stylesheets.txt
