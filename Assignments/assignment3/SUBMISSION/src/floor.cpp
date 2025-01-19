#include "floor.h"
#include "ecs.h"
#include <QDebug>

// STATIC VARS
UIElements *Floor::ui               = nullptr;
bool Floor::isFireState             = false;
int Floor::currentFloor             = 1;
bool Floor::upCalls[NUM_FLOORS];
bool Floor::downCalls[NUM_FLOORS];

Floor::Floor(UIElements *ui, QObject *parent) : QObject{parent}
{
    Floor::ui = ui;
}

void Floor::setFireState(bool newIsFireState)
{
    if (newIsFireState) {
        reset();
        qInfo() << "ECS + Floors: Now entering Emergency Disembarkation State! Recalling elevators to safe floor. (Reset simulation to exit emergency state)";
    }
    isFireState = newIsFireState;
    update();
}

int Floor::getCurrentFloor()
{
    return currentFloor;
}

bool Floor::getHasCallsFrom(int floorNum)
{
    return upCalls[floorNum - 1] || downCalls[floorNum - 1];
}

void Floor::setCurrentFloor(QComboBox *floorSelector)
{
    currentFloor = floorSelector->currentText().toInt();
    // Floor::update(); // this is done in ECS
}

bool Floor::fulfillFloorCall(int floorNum)
{
    int zero_floorNum = floorNum - 1; // make it 0-based

    if (upCalls[zero_floorNum]) {
        upCalls[zero_floorNum] = false;
        qInfo() << "Floor" << floorNum << ": Up request fulfilled!";
        return true;
    }

    if (downCalls[zero_floorNum]) {
        downCalls[zero_floorNum] = false;
        qInfo() << "Floor" << floorNum << ": Down request fulfilled!";
        return true;
    }

    return false;
}

void Floor::update()
{
    int zero_currentFloor = currentFloor - 1; // 0-based floor nummber
    bool upCalled = upCalls[zero_currentFloor];
    bool downCalled = downCalls[zero_currentFloor];

    Floor::ui->getUpButton()->setOn(upCalled);
    Floor::ui->getDownButton()->setOn(downCalled);

    Floor::ui->getUpButton()->setEnabled(!isFireState && !upCalled);
    Floor::ui->getDownButton()->setEnabled(!isFireState && !downCalled);

    Floor::ui->getFireCheckbox()->setEnabled(!isFireState);
    Floor::ui->getFireCheckbox()->setCheckState(isFireState ? Qt::Checked : Qt::Unchecked);

    Floor::ui->getPowerCheckbox()->setEnabled(!isFireState);
    Floor::ui->getPowerCheckbox()->setCheckState(isFireState ? Qt::Checked : Qt::Unchecked);

    Floor::ui->getFireStatusDisplay()->setText(isFireState ? "EVACUATION PROTOCOL" : "Operation Status: Safe");
}

void Floor::callElevator(int direction)
{
    if (direction == UP) {
        Floor::upCalls[Floor::currentFloor - 1] = true;
    }

    else if (direction == DOWN) {
        Floor::downCalls[Floor::currentFloor - 1] = true;
    }

    else {
        return; // invalid direction
    }

    ECS *ecs = ECS::instance();
    ecs->serviceFloor(currentFloor);
}

void Floor::reset()
{
    for (int i = 0; i < NUM_FLOORS; ++i) {
        upCalls[i] = false;
        downCalls[i] = false;
    }

    isFireState = false;
    update();
}
