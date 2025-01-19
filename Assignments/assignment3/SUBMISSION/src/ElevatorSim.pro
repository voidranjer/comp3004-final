QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    building.cpp \
    ecs.cpp \
    elevator.cpp \
    elevatordisplay.cpp \
    elevatorfloorsensor.cpp \
    elevatorfloorslider.cpp \
    elevatorintercom.cpp \
    floor.cpp \
    main.cpp \
    mainwindow.cpp \
    togglebutton.cpp \
    uielements.cpp

HEADERS += \
    building.h \
    defs.h \
    ecs.h \
    elevator.h \
    elevatordisplay.h \
    elevatorfloorsensor.h \
    elevatorfloorslider.h \
    elevatorintercom.h \
    floor.h \
    mainwindow.h \
    togglebutton.h \
    uielements.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
