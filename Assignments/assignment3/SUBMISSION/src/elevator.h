#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QLabel>
#include "elevatorfloorslider.h"
#include "uielements.h"

class Elevator : public QObject
{
    Q_OBJECT

public:
    // Const
    const static int OpenDoor = 1;
    const static int CloseDoor = 0;

    explicit Elevator(UIElements *ui, int elevatorNum,  QObject *parent = nullptr); // elevatorNum is 1-based
    ~Elevator();

    bool moveOneFloor(int direction); // Returns: success or not
    bool moveTowards(int floorNum); // Returns: true if elevator moved, false if elevator has arrived at destination (did not move)
    void toggleBlock();

    // getters
    bool getIsDoorOpen() const;
    int getCurrentFloor() const;
    int getElevatorNum() const;
    bool getIsBlocked() const;

    // setters
    void setCurrentFloor(int floorNum);
    void setIsDoorOpen(bool newIsDoorOpen);
    void setIsBlocked(bool newIsBlocked);

public slots:
    static void warningForTA();

private:
    UIElements *ui;
    int elevatorNum;            // 1-based indexing
    int currentFloor;           // 1-based indexing
    bool isDoorOpen;
    bool isBlocked;             // either overload or door blocked
    ElevatorFloorSlider *floorSlider;
    QLabel *floorSliderLabel;
};

#endif // ELEVATOR_H
