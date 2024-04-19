#ifndef SESSIONLOGGER_H
#define SESSIONLOGGER_H

#include <QObject>
#include <QVector>
#include "logentry.h"

class SessionLogger : public QObject
{
    Q_OBJECT
public:
    explicit SessionLogger(QObject *parent = nullptr);
    void addLogEntry(LogEntry *entry);
private:
    QVector<LogEntry*> logEntries;
signals:
};

#endif // SESSIONLOGGER_H
