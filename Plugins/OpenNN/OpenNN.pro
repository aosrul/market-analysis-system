###############################################################################
#                                                                             #
#   MAS: Market Analysis System Project                                       #
#   web.site.com                                                              #
#                                                                             #
#   O P E N N N   M A C H I N E   L E A R N I N G   P L U G I N               #
#                                                                             #
#   Aleksey Terentew                                                          #
#   My company                                                                #
#   terentew.aleksey@ya.ru                                                    #
#                                                                             #
###############################################################################

QT       -= gui

TARGET = OpenNN
TEMPLATE = lib

DEFINES += OPENNN_PLUGIN

SOURCES += opennn.cpp

HEADERS += ../global_headers/neuralnetwork_global.h \
        opennn.h

win32-g++{
    QMAKE_LFLAGS += -static-libgcc
    QMAKE_LFLAGS += -static-libstdc++
    QMAKE_LFLAGS += -static
}

unix{
target.path = /usr/lib
INSTALLS += target
}

# OpenNN library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../OpenNN/opennn/release/ -lopennn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../OpenNN/opennn/debug/ -lopennn
else:unix: LIBS += -L$$OUT_PWD/../opennn/ -lopennn

INCLUDEPATH += $$PWD/../../OpenNN/opennn
DEPENDPATH += $$PWD/../../OpenNN/opennn

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/opennn/release/libopennn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/opennn/debug/libopennn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/opennn/release/opennn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/opennn/debug/opennn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../opennn/libopennn.a

# Tiny XML 2 library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../OpenNN/tinyxml2/release/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../OpenNN/tinyxml2/debug/ -ltinyxml2
else:unix: LIBS += -L$$OUT_PWD/../tinyxml2/ -ltinyxml2
INCLUDEPATH += $$PWD/../../../OpenNN/tinyxml2
DEPENDPATH += $$PWD/../../../OpenNN/tinyxml2

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/tinyxml2/release/libtinyxml2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/tinyxml2/debug/libtinyxml2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/tinyxml2/release/tinyxml2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../OpenNN/tinyxml2/debug/tinyxml2.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/libtinyxml2.a
