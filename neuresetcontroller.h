#ifndef NEURESETCONTROLLER_H
#define NEURESETCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include "battery.h"
#include <QLabel>
#include "mainwindow.h"

class Battery;

class MainWindow;

class NeuresetController : public QObject
{
    Q_OBJECT
public:
    explicit NeuresetController(QObject *parent = nullptr, QList<QLabel *> elements = {}, MainWindow* main = nullptr);
    QDateTime getDatetime();
    bool getOutOfBattery();
    void flashBatteries();
    bool getInSession();
    bool getIsOn();

private:
    /*
     * Device Datetime
     * - To calculate the device datetime, take (QDateTime::currentDateTime() - timeOfSetting + baseDatetime)
     */
    QDateTime timeOfSetting = QDateTime::currentDateTime(); // to store the time when the device datetime was set
    QDateTime baseDatetime = QDateTime::currentDateTime(); // to store the custom time that the user sets
    void tickTime(); // tick the clock
    bool clockSettingActive = false;
    Battery* battery;
    MainWindow* main;

signals:
    void timeChanged(QDateTime datetime); // emitted every second
    void clockSettingActiveChanged(bool active);

public slots:
    void setDatetime(QDateTime datetime); // to set the device datetime
    void toggleClockSetting();
};

#endif // NEURESETCONTROLLER_H
