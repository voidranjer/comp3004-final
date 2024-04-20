#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QDebug>
#include "defs.h"
#include <QTimer>
#include <QLabel>
#include "neuresetcontroller.h"

class NeuresetController;

class Battery : public QObject
{
    Q_OBJECT

public:
    explicit Battery(QObject *parent = nullptr, QList<QLabel *> elements = {}, NeuresetController* controller = nullptr);
    bool getOutOfBattery();

signals:

public slots:
    void reduceBattery();
    void flashBatteries();

private:
    int batteryLife;
    bool outOfBattery;
    int batteryAnnouncement;
    QList<QLabel *> elements;
    void tickTime();
    NeuresetController* controller;

};

#endif // BATTERY_H
