QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    logpage.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    network.cpp \
    regis.cpp \
    service.cpp

HEADERS += \
    config_file.h \
    logpage.h \
    mainpage.h \
    mainwindow.h \
    network.h \
    regis.h \
    service.h \
    singleton.h

FORMS += \
    logpage.ui \
    mainpage.ui \
    mainwindow.ui \
    regis.ui \
    service.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
