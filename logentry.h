#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QObject>
#include <QDateTime>

class LogEntry : public QObject
{
    Q_OBJECT
public:
    explicit LogEntry(QObject *parent, QDateTime timestamp, double startingBaseline, double endingBaseline);
    QString getLogText();
private:
    QDateTime timestamp;
    double startingBaseline;
    double endingBaseline;
signals:
};

#endif // LOGENTRY_H
