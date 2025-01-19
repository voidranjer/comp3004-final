#ifndef ELEVATORDISPLAY_H
#define ELEVATORDISPLAY_H

class ElevatorDisplay
{
public:
    ElevatorDisplay();

    void evacInstructions();
    void fireWarning();
    void obstacleWarning();
    void overloadDisplay();
    void updateDisplay();

private:
    int currFloorNum;

};

#endif // ELEVATORDISPLAY_H
