###############################################################################
#                                                                             #
#   MAS: Market Analysis System Project                                       #
#   web.site.com                                                              #
#                                                                             #
#   M A R K E T   A N A L Y S I S   M A I N   P R O G R A M                   #
#                                                                             #
#   Aleksey Terentew                                                          #
#   My company                                                                #
#   terentew.aleksey@ya.ru                                                    #
#                                                                             #
###############################################################################

QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarketAnalysisSystem
TEMPLATE = app

INCLUDEPATH += $$PWD/../Plugins/global_headers/

RESOURCES += Market-Analysis.qrc

SOURCES += src/main.cpp \
        src/mainwindow.cpp \
        src/settingsform.cpp \
        src/kitconfigform.cpp \
        src/openkitdialog.cpp \
        src/presenter.cpp \
        src/settingsstruct.cpp \
        src/settingsmas.cpp \
        src/marketassaykit.cpp \
        src/neuralnetworkanalysis.cpp \
        src/imt4reader.cpp \
        src/hstreader.cpp \
        src/csvreader.cpp \
        src/csvwriter.cpp

HEADERS += include/mainwindow.h \
        include/settingsform.h \
        include/kitconfigform.h \
        include/openkitdialog.h \
        include/presenter.h \
        include/settingsstruct.h \
        include/settingsmas.h \
        include/marketassaykit.h \
        include/neuralnetworkanalysis.h \
        include/imt4reader.h \
        include/hstreader.h \
        include/csvreader.h \
        include/csvwriter.h

FORMS += mainwindow.ui \
        settingsform.ui \
        kitconfigform.ui \
        openkitdialog.ui

win32-g++{
    QMAKE_LFLAGS += -static-libgcc
    QMAKE_LFLAGS += -static-libstdc++
    QMAKE_LFLAGS += -static
}
