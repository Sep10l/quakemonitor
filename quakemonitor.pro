QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../source/repos/RaspPIconaf/src/jsonParser.c \
    ../../source/repos/RaspPIconaf/src/run.c \
    ../../source/repos/RaspPIconaf/src/object.c \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../source/repos/RaspPIconaf/src/jsonParser.h \
    ../../source/repos/RaspPIconaf/src/run.h \
    ../../source/repos/RaspPIconaf/src/object.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += C:/Users/mihne/vcpkg/installed/x64-windows/include
LIBS += -LC:/Users/mihne/vcpkg/installed/x64-windows/lib \
    -lssl \
    -lcrypto \
    -lcurl \
    -lpaho-mqtt3as \
    -lpaho-mqtt3cs \
    -lcjson \
    -lws2_32

CONFIG(debug, debug|release) {
    LIBS += -LC:/Users/mihne/vcpkg/installed/x64-windows/debug/lib
} else {
    LIBS += -LC:/Users/mihne/vcpkg/installed/x64-windows/lib
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
