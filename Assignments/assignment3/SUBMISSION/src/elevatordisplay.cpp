#include "elevatordisplay.h"
#include <QDebug>

ElevatorDisplay::ElevatorDisplay() {}

void ElevatorDisplay::evacInstructions()
{
    qInfo() << "Please evacuate the building!";
}

void ElevatorDisplay::fireWarning()
{
    qInfo() << "Fire detected! Please evacuate the building!";
}

void ElevatorDisplay::obstacleWarning()
{
    qInfo() << "Obstacle detected! Please remove it!";
}

void ElevatorDisplay::overloadDisplay()
{
    qInfo() << "Overload detected! Please remove some weight!";
}

void ElevatorDisplay::updateDisplay()
{
    qInfo() << "Display updated!";
}
