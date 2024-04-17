#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QObject>
#include <QDateTime>
#include "defs.h"

class LogEntry : public QObject
{
    Q_OBJECT
public:
    explicit LogEntry(QObject *parent = nullptr, QDateTime timestamp = QDateTime::currentDateTime(), double startingBaselines[NUM_ELECTRODES] = {0}, double endingBaselines[NUM_ELECTRODES] = {0});
private:
    QDateTime timestamp;
    double startingBaselines[NUM_ELECTRODES];
    double endingBaselines[NUM_ELECTRODES];
signals:
};

#endif // LOGENTRY_H
