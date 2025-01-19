#include <QLabel>
#include "elevatorfloorslider.h"
#include "defs.h"

ElevatorFloorSlider::ElevatorFloorSlider(QWidget *parent) : QSlider(Qt::Horizontal, parent)
{
    // setDisabled(true); // make slider read only

    // Calculate the tick interval based on the number of ticks
    int numberOfTicks = NUM_FLOORS;
    int maxTickValue = 100; // Assuming a maximum value for the slider
    int tickInterval = maxTickValue / (numberOfTicks - 1);

    setTickInterval(tickInterval);
    setTickPosition(QSlider::TicksBelow); // Set the position of the ticks below the slider
    setMinimum(0);
    setMaximum(maxTickValue);
}
