#include "logentry.h"

LogEntry::LogEntry(QObject *parent, QDateTime timestamp, double startingBaselines[NUM_ELECTRODES], double endingBaselines[NUM_ELECTRODES]) : QObject(parent)
{
    this->timestamp = timestamp;
    for (int i = 0; i < NUM_ELECTRODES; i++)
    {
        this->startingBaselines[i] = startingBaselines[i];
        this->endingBaselines[i] = endingBaselines[i];
    }
}
