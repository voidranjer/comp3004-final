#include "ecs.h"
#include "defs.h"
#include "elevator.h"
#include "floor.h"
#include <QVBoxLayout>
#include <QDebug>

// STATIC VARS
ECS *ECS::m_instance            = nullptr;

ECS::ECS(UIElements *ui, QObject *parent) : QObject{parent}, ui(ui)
{
    // Initialize other components
    building = new Building(ui, this);

    // Floor: Init floor selector
    QComboBox *floorSelector = ui->getFloorSelector();
    for (int i = 0; i < NUM_FLOORS; ++i) {
        floorSelector->addItem(QString::number(i + 1));
    }
    connect(floorSelector, QOverload<int>::of(&QComboBox::activated), this, [this, floorSelector](int index) {
        // this->setCurrentFloor(index + 1); // Adding 1 because you start numbering from 1
        setCurrentFloor(floorSelector);
    });


    // Init destination buttons
    for (int i = 0; i < NUM_FLOORS; ++i) {
        ToggleButton *destButton = new ToggleButton(QString::number(i + 1), ToggleButton::AutoDisable);
        destButton->setEnabled(false);
        connect(destButton, &ToggleButton::clicked, this, [i, this](){ this->setDestination(i + 1); });

        ui->addDestButton(destButton, i + 1);
    }

    // Init door controls
    QPushButton *openButton = ui->getOpenButton();
    connect(openButton, &QPushButton::clicked, this, [this](){ this->doorOverride(Elevator::OpenDoor); });
    QPushButton *closeButton = ui->getCloseButton();
    connect(closeButton, &QPushButton::clicked, this, [this](){ this->doorOverride(Elevator::CloseDoor); });

    // Init safety features
    QCheckBox *fireCheckbox = ui->getFireCheckbox();
    connect(fireCheckbox, &QCheckBox::clicked, this, &ECS::fireProtocol);
    QCheckBox *powerCheckbox = ui->getPowerCheckbox();
    connect(powerCheckbox, &QCheckBox::clicked, this, &ECS::fireProtocol);
    QPushButton *intercomButton = ui->getIntercomButton();
    connect(intercomButton, &QPushButton::clicked, this, &ECS::connectIntercom);
    QPushButton *intercomDisconnectButton = ui->getIntercomDisconnectButton();
    connect(intercomDisconnectButton, &QPushButton::clicked, building, &Building::disconnectIntercom);
    QCheckBox *obstacleCheckbox = ui->getObstacleCheckbox();
    connect(obstacleCheckbox, &QCheckBox::clicked, this, &ECS::toggleBlockElevator);
    QCheckBox *overloadCheckbox = ui->getOverloadCheckbox();
    connect(overloadCheckbox, &QCheckBox::clicked, this, &ECS::toggleBlockElevator);

    // IMPORTANT: Call this AFTER all initialization (otherwise odd untraceable crashes)
    reset();
}

ECS::~ECS()
{

}

ECS *ECS::instance(UIElements *ui)
{
    // Create instance if it doesn't exist
    if (!m_instance)
        m_instance = new ECS(ui);

    return m_instance;
}

ECS *ECS::instance()
{
    if (m_instance == nullptr) {
        qInfo() << "Warning!!! ECS::instance() has just returned a nullptr. Could lead to obscure crashes!";
    }

    return m_instance;
}

void ECS::destroyInstance()
{
    // Delete the singleton instance if it exists
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void ECS::serviceFloor(int floorNum)
{
    qInfo() << "ECS: Added Floor" << Floor::getCurrentFloor() << "to queue...";
    callQueue.enqueue(floorNum);
}

void ECS::setDestination(int elevatorNum, int floorNum)
{
    destinations[elevatorNum - 1] = floorNum;
}

void ECS::setDestination(int floorNum)
{
    setDestination(currentElevator, floorNum);
}

Elevator *ECS::getCurrentElevator()
{
    return elevators[currentElevator - 1];
}

Elevator *ECS::getElevator(int elevatorNum)
{
    return elevators[elevatorNum - 1];
}

int ECS::getDestOfCurrentElevator()
{
    return destinations[currentElevator - 1];
}

void ECS::handleElevatorChange(const QString &text)
{
    currentElevator = text.toInt();
    update();
}

void ECS::setCurrentFloor(QComboBox *floorSelector)
{
    Floor::setCurrentFloor(floorSelector);
    update();
}

void ECS::updateTick()
{
    // dispatch free elevator to caller's floor
    if (!callQueue.isEmpty()) {
        int floorToService = callQueue.dequeue();
        for (int i = 0; i < NUM_ELEVATORS; ++i) {
            if (destinations[i] == NO_DESTINATION) {
                setDestination(i + 1, floorToService);
                qInfo() << "ECS: Dispatched Elevator" << (i + 1) << "to Floor" << floorToService;
                break;
            }
        }
    }

    // move elevators to destinations (STATE MACHINE)
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        int destinationFloor = destinations[i];

        if (destinationFloor == NO_DESTINATION) {

        }

        else if (destinationFloor == WAITING_FOR_DEST) {

        }

        else {
            bool didMove = elevators[i]->moveTowards(destinationFloor);

            // arrived at destination
            if (!didMove) {
                // Open doors if not already open
                if (!elevators[i]->getIsDoorOpen()) {
                    elevators[i]->setIsDoorOpen(true);
                }

                // Elevator fulfilling UP/DOWN call request
                if (Floor::getHasCallsFrom(destinationFloor)) {
                    Floor::fulfillFloorCall(destinationFloor);
                    destinations[i] = WAITING_FOR_DEST;
                }

                // Elevator arriving at destination requested by a boarded passenger
                else {
                    destinations[i] = NO_DESTINATION;
                }
            }
        }
    }

    // rerender
    update();

}

void ECS::fireProtocol()
{
    // recall all elevators to safe floor
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        destinations[i] = SAFE_FLOOR;
    }

    // reset and disable call buttons
    Floor::setFireState(true);
}

void ECS::connectIntercom()
{
    Elevator *currentElevator = getCurrentElevator();
    building->establishIntercomConnection(currentElevator);
}

void ECS::doorOverride(int isDoorOpen)
{
    getCurrentElevator()->setIsDoorOpen(isDoorOpen == Elevator::OpenDoor);
    // update();
}

void ECS::toggleBlockElevator()
{
    getCurrentElevator()->toggleBlock();
    // update();
}

void ECS::update()
{
    Floor::update();
    ui->update();
}

void ECS::reset()
{
    // Reset state
    currentElevator = 1;
    callQueue.clear();

    // Free previous memory
    for (Elevator *elevator : elevators) {
        if (elevator != nullptr) {
            delete elevator;
        }
    }

    // Re-initialize elevators
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        elevators[i] = new Elevator(ui, i + 1);
    }

    // Reset destinations
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        destinations[i] = NO_DESTINATION;
    }

    // Reset other components
    Floor::reset();
    ui->reset();
    building->reset();

    qInfo() << "Simulation has been reset!";
}
