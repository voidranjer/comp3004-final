#include "building.h"
#include <QDebug>

Building::Building(UIElements *ui, QObject *parent) : QObject{parent}, ui{ui}
{
    intercomConnectedElevator = nullptr;
}

void Building::establishIntercomConnection(Elevator *elevator)
{
    intercomConnectedElevator = elevator;

    int elevatorNum = elevator->getElevatorNum();

    ui->getIntercomStatusDisplay()->setText(QString("Conn. -> Channel %1").arg(elevatorNum));
    ui->getIntercomRecvStatusDisplay()->setText(QString("Conn. -> Elevator %1").arg(elevatorNum));
    ui->getIntercomDisconnectButton()->setEnabled(true);

    qInfo() << "Building Control: Connected to elevator " << elevatorNum;
}

void Building::reset()
{
    disconnectIntercom();
}

void Building::disconnectIntercom()
{
    intercomConnectedElevator = nullptr;

    ui->getIntercomStatusDisplay()->setText("Disconnected");
    ui->getIntercomRecvStatusDisplay()->setText("Disconnected");
    ui->getIntercomDisconnectButton()->setEnabled(false);

    qInfo() << "Building Control: Intercom disconnected!";
}

