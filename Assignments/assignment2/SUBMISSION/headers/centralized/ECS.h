#ifndef ECS_H
#define ECS_H

#include <map>
#include <vector>

#include "Elevator.h"
#include "Floor.h"

class ECS {
   public:
    // Singleton
    static ECS& getInstance() {
        static ECS instance;
        return instance;
    }

    // Success 1
    void floorRequest(int floorNum, int direction);
    void selectElevator(int floorNum);
    void reportNewFloor(int floorNum, int elevatorNum);
    void makeDecision(int floorNum, int elevatorNum);  // decision on whether or not to stop the elevator at the floor, then calls the 'stop' method of Elevator

    // Success 2
    void requestDestination(int floorNum, int elevatorNum);
    void readyToMove(int elevatorNum);  // Elevator will call this method when it is ready to move to the next floor, and this method will call the 'move' method of Elevator

    // Safety 1
    void help(int elevatorNum);  // intercom
    void openChannel(int elevatorNum);
    void checkConnection(int elevatorNum);  // if no connection after 1 minute, invoke 'dialEmergency' from 911 class

    // Safety 2
    void doorBlocked(int elevatorNum);
    void doorUnblocked(int elevatorNum);

    // Safety 3
    void triggerFireState();

    // Safety 4
    void reportOverload(int elevatorNum);
    void clearOverload(int elevatorNum);

    // Safety 5
    void triggerPowerOutage();

   private:
    std::vector<Floor> servicedFloors;
    std::vector<Elevator> elevators;
    std::map<Elevator, std::vector<Floor>> destinations;  // Elevator: [floorNum1, floorNum2, ...]
    std::map<Elevator, Floor> currentFloor;               // Elevator: floorNum
};

#endif