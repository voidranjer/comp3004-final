#ifndef ECS_H
#define ECS_H

#include <QObject>
#include <QPushButton>
#include <QVector>
#include <QSlider>
#include <QQueue>
#include <QGridLayout>
#include <QCheckBox>
#include "defs.h"
#include "elevator.h"
#include "building.h"

class ECS : public QObject
{
    Q_OBJECT

public:
    ECS(UIElements *uiElements, QObject *parent = nullptr);
    ~ECS();

    // Singleton (Design Pattern)
    static ECS *instance(UIElements *uiElements);
    static ECS *instance(); // this only works if the instance has already been created. otherwise returns nullptr
    static void destroyInstance();

    // use `serviceFloor` to automatically dispatch elevator to a floor, use setDestination to send a specific elevator to a floor
    void serviceFloor(int floorNum);     // floorNum is 1-based. (this function is direction-agnostic, it queues a floor to be serviced by an elevator)
    void setDestination(int elevatorNum, int floorNum);   //floorNum is 1-based
    void setDestination(int floorNum); // override to use currentElevator

    Elevator *getCurrentElevator();
    Elevator *getElevator(int elevatorNum);
    int getDestOfCurrentElevator();

public slots:
    void handleElevatorChange(const QString &text);
    void setCurrentFloor(QComboBox *floorSelector);
    void updateTick(); // Runs at a specified interval using QTimer
    void fireProtocol(); // On execution, recall all elevators to safe floor and disable floor call buttons
    void connectIntercom();
    void doorOverride(int isDoorOpen);
    void toggleBlockElevator();
    void update();
    void reset(); // Reset simulation

private:
    static ECS *m_instance;
    UIElements *ui;
    Building *building;

    // Elevators
    Elevator *elevators[NUM_ELEVATORS];
    int currentElevator; // 1-based indexing
    int destinations[NUM_ELEVATORS];
    QQueue<int> callQueue; // Queue for the floor calls in case there are no free elevators at the time of call

};

#endif // ECS_H
