#ifndef FLOOR_H
#define FLOOR_H

#include <vector>

#include "Button.h"

class Floor {
   public:
    void inform(int direction);
    void serviced(int direction);
    void suspendServices();

   private:
    class FloorButton : public Button {
       private:
        int direction;
    };

    int floorNum;
    std::vector<FloorButton> callButtons;
};

#endif