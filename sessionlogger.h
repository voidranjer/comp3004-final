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
    QVector<LogEntry *> getLogEntries() const;

public slots:
    QString addEntry(QDateTime timestamp, double startingBaseline, double endingBaseline);
private:
    QVector<LogEntry*> logEntries;
};

#endif // SESSIONLOGGER_H
