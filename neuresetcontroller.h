#ifndef NEURESETCONTROLLER_H
#define NEURESETCONTROLLER_H

#include <QObject>
#include <QDateTime>

class NeuresetController : public QObject
{
    Q_OBJECT
public:
    explicit NeuresetController(QObject *parent = nullptr);
private:

    /*
     * Device Datetime
     * - To calculate the device datetime, take (QDateTime::currentDateTime() - timeOfSetting + baseDatetime)
     */
    QDateTime timeOfSetting; // to store the time when the device datetime was set
    QDateTime baseDatetime = QDateTime::currentDateTime(); // to store the custom time that the user sets

signals:
};

#endif // NEURESETCONTROLLER_H
