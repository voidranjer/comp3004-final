#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include "togglebutton.h"
#include "defs.h"
#include "uielements.h"

class Floor : public QObject
{
    Q_OBJECT

public:
    explicit Floor(UIElements *ui, QObject *parent = nullptr);

    // getters
    static int getCurrentFloor();
    static bool getHasCallsFrom(int floorNum);       // floorNum is 1-based (returns true if this floor has either UP/DOWN call requests)

    // setters
    static void setFireState(bool isFireState);
    static bool fulfillFloorCall(int floorNum);     // floorNum is 1-based. As well, this function naively fulfills call requests with a bias towards "UP" requests. (blindly fulfill "UP"s first, then "DOWN"s after). If there were no existing calls, then return false (elevator arrival caused by boarded passenger's request, not floor request.

    // setup
    static void init(QComboBox *floorSelector, QVBoxLayout *callButtonsContainer, QCheckBox *powerCheckbox, QCheckBox *fireCheckbox, QLineEdit *fireStatusDisplay);
    static void reset();     // on reset simulation (or call this during fire alarm BEFORE setEnabled(false))
    static void update();    // to be run on each tick (rerender)

    // misc
    static void callElevator(int direction);

public slots:
    static void setCurrentFloor(QComboBox *floorSelector);

private:
    static UIElements *ui;
    static bool isFireState;                // used by fire alarm to disable calling the elevators
    static int currentFloor;                // current floor of the floor selector
    static bool upCalls[NUM_FLOORS];        // Index of boolean represents the corresponding floor level (True means this direction is activated/called) [remember to 'i - 1' when accessing this 0-based index]
    static bool downCalls[NUM_FLOORS];

signals:
};

#endif // FLOOR_H
