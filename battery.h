#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>

class Battery : public QObject
{
    Q_OBJECT
public:
    explicit Battery(QObject *parent = nullptr);

signals:

};

#endif // BATTERY_H
