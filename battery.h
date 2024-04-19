#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QDebug>
#include "defs.h"
#include <QTimer>
#include <QLabel>

class Battery : public QObject
{
    Q_OBJECT

public:
    explicit Battery(QObject *parent = nullptr, QList<QLabel *> elements = {});
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

};

#endif // BATTERY_H
