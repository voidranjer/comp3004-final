#include "neuresetcontroller.h"
#include <QTimer>
#include <QDebug>
#include "defs.h"

NeuresetController::NeuresetController(QObject *parent)
    : QObject{parent}
{
    // Datetime ticker
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){ tickTime(); });
    timer->start(CLOCK_TICK);
}

QDateTime NeuresetController::getDatetime()
{
    // To calculate the device datetime, take (QDateTime::currentDateTime() - timeOfSetting + baseDatetime)
    qint64 secondsElapsed = timeOfSetting.secsTo(QDateTime::currentDateTime());
    QDateTime currDatetime = baseDatetime.addSecs(secondsElapsed);
    return currDatetime;
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
