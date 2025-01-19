#include "elevatorfloorsensor.h"
#include <QDebug>

ElevatorFloorSensor::ElevatorFloorSensor() {}

void ElevatorFloorSensor::detectMe()
{
    qInfo() << "Floor detected!";
}
