###############################################################################
#                                                                             #
#   Market Analysis System                                                    #
#   https://www.mql5.com/ru/users/terentjew23                                 #
#                                                                             #
#   M A R K E T   A N A L Y S I S   P R O G R A M                             #
#                                                                             #
#   Aleksey Terentew                                                          #
#   terentew.aleksey@ya.ru                                                    #
#                                                                             #
###############################################################################

QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = Market-Analysis
TEMPLATE  = app
CONFIG   += c++11

RC_FILE   = Market-Analysis.rc
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
        src/masopennnclasses.cpp \
        src/imt4reader.cpp \
        src/hstreader.cpp \
        src/csvreader.cpp \
        src/csvwriter.cpp \
        src/csvpredictionwriter.cpp

HEADERS += include/mainwindow.h \
        include/settingsform.h \
        include/kitconfigform.h \
        include/openkitdialog.h \
        include/presenter.h \
        include/settingsstruct.h \
        include/settingsmas.h \
        include/marketassaykit.h \
        include/neuralnetworkanalysis.h \
        include/masopennnclasses.h \
        include/imt4reader.h \
        include/hstreader.h \
        include/csvreader.h \
        include/csvwriter.h \
        include/csvpredictionwriter.h

FORMS += mainwindow.ui \
        settingsform.ui \
        kitconfigform.ui \
        openkitdialog.ui

win32-g++{
    QMAKE_LFLAGS += -static-libgcc
    QMAKE_LFLAGS += -static-libstdc++
    QMAKE_LFLAGS += -static
}

# OpenMP library

win32:!win32-g++{
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS   += -openmp
}else{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS   += -fopenmp
}

# OpenNN library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../OpenNN/opennn/release/ -lopennn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../OpenNN/opennn/debug/ -lopennn
else:unix: LIBS += -L$$OUT_PWD/../opennn/ -lopennn

INCLUDEPATH += $$PWD/../../OpenNN/opennn
DEPENDPATH += $$PWD/../../OpenNN/opennn

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/opennn/release/libopennn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/opennn/debug/libopennn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/opennn/release/opennn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/opennn/debug/opennn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../opennn/libopennn.a

# Tiny XML 2 library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../OpenNN/tinyxml2/release/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../OpenNN/tinyxml2/debug/ -ltinyxml2
else:unix: LIBS += -L$$OUT_PWD/../tinyxml2/ -ltinyxml2

INCLUDEPATH += $$PWD/../../OpenNN/tinyxml2
DEPENDPATH += $$PWD/../../OpenNN/tinyxml2

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/tinyxml2/release/libtinyxml2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/tinyxml2/debug/libtinyxml2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/tinyxml2/release/tinyxml2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../OpenNN/tinyxml2/debug/tinyxml2.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/libtinyxml2.a
