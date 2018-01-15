#-------------------------------------------------
#
# Project created by QtCreator 2014-06-18T19:58:57
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

QMAKE_CXXFLAGS += -Wno-deprecated

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisualGPSqt
TEMPLATE = app
# Windows Icon
RC_FILE = VisualGPSQt.rc
# Mac Icon
ICON = Resources/icon.icns

SOURCES += main.cpp\
        MainWindow.cpp \
    NMEAParserQt.cpp \
    ConnectDlg.cpp \
    AboutDlg.cpp \
    GPSStatusWnd.cpp \
    SignalQuality.cpp \
    AzimuthElevationWnd.cpp \
    PositionStatusWnd.cpp \
    LEDWnd.cpp \
    ../../NMEAParserLib/NMEAParser.cpp \
    ../../NMEAParserLib/NMEAParserPacket.cpp \
    ../../NMEAParserLib/NMEASentenceBase.cpp \
    ../../NMEAParserLib/NMEASentenceGGA.cpp \
    ../../NMEAParserLib/NMEASentenceGSA.cpp \
    ../../NMEAParserLib/NMEASentenceGSV.cpp \
    ../../NMEAParserLib/NMEASentenceRMC.cpp

HEADERS  += MainWindow.h \
    NMEAParserQt.h \
    ConnectDlg.h \
    AboutDlg.h \
    GPSStatusWnd.h \
    SignalQuality.h \
    AzimuthElevationWnd.h \
    PositionStatusWnd.h \
    LEDWnd.h \
    ../../NMEAParserLib/NMEAParser.h \
    ../../NMEAParserLib/NMEAParserData.h \
    ../../NMEAParserLib/NMEAParserPacket.h \
    ../../NMEAParserLib/NMEASentenceBase.h \
    ../../NMEAParserLib/NMEASentenceGGA.h \
    ../../NMEAParserLib/NMEASentenceGSA.h \
    ../../NMEAParserLib/NMEASentenceGSV.h \
    ../../NMEAParserLib/NMEASentenceRMC.h

FORMS    += MainWindow.ui \
    ConnectDlg.ui \
    AboutDlg.ui

RESOURCES += \
    Images.qrc
