#include "neuresetcontroller.h"
#include <QTimer>
#include <QDebug>
#include "defs.h"

NeuresetController::NeuresetController(QObject *parent, QList<QLabel *> elements, MainWindow* main)
    : QObject{parent}, main(main)
{
    // Datetime ticker
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){ tickTime(); });
    timer->start(CLOCK_TICK);

    battery = new Battery(this, elements, this);
    sessionLogger = new SessionLogger(this);
}

QDateTime NeuresetController::getDatetime()
{
    // To calculate the device datetime, take (QDateTime::currentDateTime() - timeOfSetting + baseDatetime)
    qint64 secondsElapsed = timeOfSetting.secsTo(QDateTime::currentDateTime());
    QDateTime currDatetime = baseDatetime.addSecs(secondsElapsed);
    return currDatetime;
}

bool NeuresetController::getOutOfBattery()
{
    return battery->getOutOfBattery();
}

void NeuresetController::flashBatteries()
{
    battery->flashBatteries();
}

void NeuresetController::tickTime()
{
    QDateTime currDatetime = getDatetime();
    emit timeChanged(currDatetime);
}

void NeuresetController::setDatetime(QDateTime datetime)
{
    timeOfSetting = QDateTime::currentDateTime();
    baseDatetime = datetime;

    qDebug() << "Controller: Updated time setting!";
}

void NeuresetController::toggleClockSetting()
{
    clockSettingActive = !clockSettingActive;
    emit clockSettingActiveChanged(clockSettingActive);
}

void NeuresetController::handleSessionCompleted(double startingBaseline, double endingBaseline)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    qDebug() << "Controller:" << timestamp.toString("yyyy-MM-dd hh:mm:ss") << "Adding session to log...";
}

bool NeuresetController::getInSession() {
    return main->getInSession();
}

bool NeuresetController::getIsOn()
{
    return main->getIsOn();
}


