#include "sessionlogger.h"

SessionLogger::SessionLogger(QObject *parent)
    : QObject{parent}
{}

void SessionLogger::addLogEntry(LogEntry *entry)
{
    logEntries.append(entry);
}
