#include "logentry.h"

LogEntry::LogEntry(QObject *parent, QDateTime timestamp, double startingBaseline, double endingBaseline) : QObject(parent)
{
    this->timestamp = timestamp;
    this->startingBaseline = startingBaseline;
    this->endingBaseline = endingBaseline;
}
