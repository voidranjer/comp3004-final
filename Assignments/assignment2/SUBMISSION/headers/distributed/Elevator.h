#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>

#include "Button.h"
#include "Protocols.h"

class ElevatorDoor {
   public:
    void open();
    void close();
    void blocked();

   private:
    bool isOpen;
};

class ElevatorFloorSensor {
   private:
    friend class Floor;
    void detectMe(int floorNum);  // to be called by floors
};

class ElevatorBell {
   public:
    void ring();
};

class ElevatorDestinationButton : public Button {
   private:
    int floorNum;
};

class ElevatorIntercom {
   public:
    void blinkIndicator();
    void establishVoice();
};

class ElevatorHelpButton : public Button {
   public:
    void help();
};

class ElevatorDisplay : WithSafetyProtocols {
   public:
    void updateDisplay(int floorNum);
    void obstacleWarning() override;
    void fireWarning() override;
    void overloadWarning() override;
    void evacInstructions() override;

   private:
    int currFloorNum;
};

class ElevatorAudio : WithSafetyProtocols {
   public:
    void obstacleWarning() override;
    void fireWarning() override;
    void overloadWarning() override;
    void evacInstructions() override;
};

class ElevatorWeight {
   public:
    void updateWeight(int weight);
};

class Elevator {
   public:
    // Success 1
    void move(int direction);
    void updateCurrentFloor(int floorNum);
    void stop();  // should be a macro for move(STOP);
    void floorRequest(int floorNum);
    void decideIfMe();  // decides if this elevator should be dispatched to the floor (by comparing distances to other elevators using the static 'distances' vector)
    void updateFloor(int floorNum);
    void makeDecision();  // decision on whether or not to stop the elevator at the floor, then calls the 'stop' method of Elevator

    // Success 2
    void requestDestination(int floorNum);  // called by Elevator Destination Panel
    void readyToMove();

    // Safety 1
    void help();
    void openChannel();
    void help();  // intercom
    void openChannel();
    void checkConnection();  // if no connection after 1 minute, invoke 'dialEmergency' from 911 class

    // Safety 2
    void doorBlocked();
    void doorUnblocked();
    void unblockDoorThenReportBack();
    void resumeFunction();

    // Safety 3
    void fireWarning();
    void triggerFireState();

    // Safety 4
    void overloadThresh();
    void clearOverload();
    void suspendSequence();  // due to overload
    void resumeSequence();

    // Safety 5
    void evacProtocol();
    void triggerPowerOutage();

   private:
    // the necessity of some of these states should be reconsidered
    int currFloorNum;
    int currDirection;
    bool doorBlocked;
    bool fireState;
    bool overloadState;
    bool powerOutageState;

    static std::vector<int> distances;  // dist between elevators to target floor (index == elevatorNum)
    int currentFloor;                   // unique to "distributed", otherwise stored in ECS

    ElevatorDoor door;
    ElevatorFloorSensor floorSensor;
    ElevatorBell bell;
    ElevatorDisplay display;
    ElevatorAudio audio;
    ElevatorIntercom intercom;
    ElevatorHelpButton helpButton;
    ElevatorWeight weightSensor;
    std::vector<ElevatorDestinationButton> destinationPanel;
};

#endif