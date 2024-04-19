QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    electrode.cpp \
    logentry.cpp \
    eegsimulator.cpp \
    main.cpp \
    mainwindow.cpp \
    neuresetcontroller.cpp \
    qcustomplot.cpp \
    pcwindow.cpp

HEADERS += \
    defs.h \
    electrode.h \
    logentry.h \
    eegsimulator.h \
    mainwindow.h \
    neuresetcontroller.h \
    pcwindow.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui \
    pcwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
