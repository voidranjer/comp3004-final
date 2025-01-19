#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include "uielements.h"
#include "elevator.h"

class Building : public QObject
{
    Q_OBJECT

public:
    explicit Building(UIElements *ui, QObject *parent = nullptr);
    void establishIntercomConnection(Elevator *elevator);
    void reset();

public slots:
    void disconnectIntercom();

private:
    UIElements *ui;
    Elevator *intercomConnectedElevator;

signals:
};

#endif // BUILDING_H
