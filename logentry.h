#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QObject>

class LogEntry : public QObject
{
    Q_OBJECT
public:
    explicit LogEntry(QObject *parent = nullptr);

signals:
};

#endif // LOGENTRY_H
