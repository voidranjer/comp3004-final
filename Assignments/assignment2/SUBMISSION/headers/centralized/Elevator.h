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

    // Success 2
    void requestDestination(int floorNum);  // called by Elevator Destination Panel

    // Safety 1
    void help();
    void openChannel();

    // Safety 2
    void doorBlocked();
    void doorUnblocked();
    void unblockDoorThenReportBack();
    void resumeFunction();

    // Safety 3
    void fireWarning();

    // Safety 4
    void overloadThresh();
    void clearOverload();
    void suspendSequence();  // due to overload
    void resumeSequence();

    // Safety 5
    void evacProtocol();

   private:
    // the necessity of some of these states should be reconsidered
    int currFloorNum;
    int currDirection;
    bool doorBlocked;
    bool fireState;
    bool overloadState;
    bool powerOutageState;

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