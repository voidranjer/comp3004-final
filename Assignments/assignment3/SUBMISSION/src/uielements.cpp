#include "uielements.h"
#include "ecs.h"
#include "floor.h"

UIElements::UIElements(MainWindow *mainWindow, QObject *parent) : QObject{parent}, mainWindow{mainWindow}
{
    // Set properties (read only)
    mainWindow->ui->intercomRecvStatusDisplay->setReadOnly(true);
    mainWindow->ui->fireStatusDisplay->setReadOnly(true);

    // Floor: Init call buttons
    QVBoxLayout *callButtonsContainer = mainWindow->ui->callButtonsContainer;
    upButton = new ToggleButton("Up", ToggleButton::AutoDisable);
    downButton = new ToggleButton("Down", ToggleButton::AutoDisable);
    callButtonsContainer->addWidget(upButton);
    callButtonsContainer->addWidget(downButton);
    connect(upButton, &ToggleButton::clicked, upButton, [](){ Floor::callElevator(UP); });
    connect(downButton, &ToggleButton::clicked, downButton, [](){ Floor::callElevator(DOWN); });
}

QPushButton *UIElements::getIntercomButton()
{
    return mainWindow->ui->intercomButton;
}

QLCDNumber *UIElements::getFloorNumberDisplay()
{
    return mainWindow->ui->floorNumberDisplay;
}

QGridLayout *UIElements::getSliderContainer()
{
    return mainWindow->ui->sliderContainer;
}

QVBoxLayout *UIElements::getDestButtonsContainer()
{
    return mainWindow->ui->destButtonsContainer;
}

QLineEdit *UIElements::getDoorStatusDisplay()
{
    return mainWindow->ui->doorStatusDisplay;
}

QPushButton *UIElements::getOpenButton()
{
    return mainWindow->ui->openButton;
}

QPushButton *UIElements::getCloseButton()
{
    return mainWindow->ui->closeButton;
}

QCheckBox *UIElements::getOverloadCheckbox()
{
    return mainWindow->ui->overloadCheckbox;
}

QCheckBox *UIElements::getObstacleCheckbox()
{
    return mainWindow->ui->obstacleCheckbox;
}

void UIElements::addDestButton(ToggleButton *destButton, int floorNum)
{
    destinationButtons[floorNum - 1] = destButton;
    getDestButtonsContainer()->addWidget(destButton);
}

QLineEdit *UIElements::getIntercomStatusDisplay()
{
    return mainWindow->ui->intercomStatusDisplay;
}

QLineEdit *UIElements::getIntercomRecvStatusDisplay()
{
    return mainWindow->ui->intercomRecvStatusDisplay;
}

QPushButton *UIElements::getIntercomDisconnectButton()
{
    return mainWindow->ui->intercomDisconnectButton;
}

ToggleButton *UIElements::getUpButton()
{
    return upButton;
}

ToggleButton *UIElements::getDownButton()
{
    return downButton;
}

QCheckBox *UIElements::getFireCheckbox()
{
    return mainWindow->ui->fireCheckbox;
}

QCheckBox *UIElements::getPowerCheckbox()
{
    return mainWindow->ui->powerCheckbox;
}

QLineEdit *UIElements::getFireStatusDisplay()
{
    return mainWindow->ui->fireStatusDisplay;
}

QComboBox *UIElements::getFloorSelector()
{
    return mainWindow->ui->floorSelector;
}

void UIElements::update()
{
    // Get ECS state
    ECS *ecs = ECS::instance();
    int destinationTarget = ecs->getDestOfCurrentElevator();
    Elevator *currentElevator = ecs->getCurrentElevator();

    // Rerender elevator destination buttons
    for (int i = 0; i < NUM_FLOORS; ++i) {
        bool isDest = (destinationTarget == i + 1);
        destinationButtons[i]->setOn(isDest);
        destinationButtons[i]->setEnabled(destinationTarget == WAITING_FOR_DEST);
    }

    // Rerender elevator floor display
    int floorNum = currentElevator->getCurrentFloor();
    getFloorNumberDisplay()->display(QString::number(floorNum));

    // Rerender elevator door status display
    bool isBlocked = currentElevator->getIsBlocked();
    bool isDoorOpen = currentElevator->getIsDoorOpen();
    getDoorStatusDisplay()->setText(isBlocked ? "OVERLOAD/BLOCKED" : isDoorOpen ? "Door: Open" : "Door: Closed");

}

void UIElements::reset()
{
    // Reset blocked checkboxes
    getObstacleCheckbox()->setChecked(false);
    getOverloadCheckbox()->setChecked(false);
}

