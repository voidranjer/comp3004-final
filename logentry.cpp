#include "logentry.h"

LogEntry::LogEntry(QObject *parent, QDateTime timestamp, double startingBaseline, double endingBaseline) : QObject(parent)
{
    this->timestamp = timestamp;
    this->startingBaseline = startingBaseline;
    this->endingBaseline = endingBaseline;
}

QString LogEntry::getLogText()
{
    QString logText = timestamp.toString("yyyy-MM-dd hh:mm:ss") + " - " + QString::number(startingBaseline) + " --> " + QString::number(endingBaseline);
    return logText;
}
