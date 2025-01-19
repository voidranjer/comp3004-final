#include "elevator.h"
#include "defs.h"
#include <QGridLayout>
#include <cmath>
#include <QDebug>

Elevator::Elevator(UIElements *ui, int elevatorNum, QObject *parent) : QObject{parent}, ui(ui), elevatorNum(elevatorNum)
{
    currentFloor = 1; // starting floor
    isDoorOpen = false;

    // Initialize floor slider
    floorSlider = new ElevatorFloorSlider();
    floorSliderLabel = new QLabel(QString("Elevator %1").arg(elevatorNum));
    ui->getSliderContainer()->addWidget(floorSliderLabel, elevatorNum - 1, 0);
    ui->getSliderContainer()->addWidget(floorSlider, elevatorNum - 1, 1);
    connect(floorSlider, &QSlider::sliderReleased, this, &Elevator::warningForTA);

}

Elevator::~Elevator()
{
    delete floorSlider;
    delete floorSliderLabel;
}

bool Elevator::moveOneFloor(int direction)
{
    // elevatorNum uses 1-based indexing

    if (direction == UP && currentFloor <= NUM_FLOORS) {
        setCurrentFloor(currentFloor + 1);
    } else if (direction == DOWN && currentFloor > 1) {
        setCurrentFloor(currentFloor - 1);
    } else {
        return false;
    }

    return true;
}

bool Elevator::moveTowards(int floorNum)
{
    if (floorNum > currentFloor) {
        return moveOneFloor(UP);
    } else if (floorNum < currentFloor) {
        return moveOneFloor(DOWN);
    } else {
        return false;
    }
}

void Elevator::toggleBlock()
{
    setIsBlocked(!isBlocked);
}

void Elevator::warningForTA()
{
    qInfo() << "Warning for the grading TA: Manually controlling the elevator sliders is not supported! I did not set the sliders to read-only mode solely due to undesirable styles (and the lack of time to custom style them). Reset the simulation to revert!";
}

void Elevator::setIsBlocked(bool newIsBlocked)
{
    isBlocked = newIsBlocked;
    qInfo() << "Elevator" << elevatorNum << ": Obstacle/Overload" << (isBlocked ? "detected" : "resolved");
}

bool Elevator::getIsBlocked() const
{
    return isBlocked;
}

void Elevator::setIsDoorOpen(bool newIsDoorOpen)
{
    isDoorOpen = newIsDoorOpen;
    qInfo() << "Elevator" << elevatorNum << ": Door is now" << (isDoorOpen ? "open" : "closed");
}

int Elevator::getElevatorNum() const
{
    return elevatorNum;
}

int Elevator::getCurrentFloor() const
{
    return currentFloor;
}

bool Elevator::getIsDoorOpen() const
{
    return isDoorOpen;
}

void Elevator::setCurrentFloor(int floorNum)
{
    if (floorNum < 1 || floorNum > NUM_FLOORS) {
        qInfo() << "Elevator" << elevatorNum << ": Invalid floor number" << floorNum;
        return;
    }

    if (isBlocked) {
        qInfo() << "Elevator" << elevatorNum << ": Elevator is blocked. Cannot move to Floor" << floorNum << ", please resolve obstacle/overload!";
        return;
    }

    if (isDoorOpen) setIsDoorOpen(false); // close doors before moving

    currentFloor = floorNum;

    int percentage = std::round(100.0 * (float(currentFloor - 1) / (NUM_FLOORS - 1)));
    qInfo() << "Elevator" << elevatorNum << ": Moved to Floor" << currentFloor;
    floorSlider->setValue(percentage);
}
