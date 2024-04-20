#include "sessionlogger.h"

SessionLogger::SessionLogger(QObject *parent)
    : QObject{parent}
{}

QString SessionLogger::addEntry(QDateTime timestamp, double startingBaseline, double endingBaseline)
{
    LogEntry* logEntry = new LogEntry(this, timestamp, startingBaseline, endingBaseline);
    logEntries.append(logEntry);
    QString logText = "Session: " + timestamp.toString("yyyy-MM-dd hh:mm:ss");
    return logText;
}

QVector<LogEntry *> SessionLogger::getLogEntries() const
{
    return logEntries;
}
