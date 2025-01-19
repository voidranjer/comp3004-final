#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QLCDNumber>
#include "defs.h"
#include "togglebutton.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class UIElements : public QObject
{
    Q_OBJECT
public:
    explicit UIElements(MainWindow *mainWindow, QObject *parent = nullptr);

    // Elevator
    QPushButton* getIntercomButton();
    QLCDNumber *getFloorNumberDisplay();
    QGridLayout *getSliderContainer();
    QVBoxLayout *getDestButtonsContainer();
    QLineEdit *getDoorStatusDisplay();
    QPushButton *getOpenButton();
    QPushButton *getCloseButton();
    QCheckBox *getOverloadCheckbox();
    QCheckBox *getObstacleCheckbox();
    void addDestButton(ToggleButton *destButton, int floorNum);

    // Building
    QLineEdit *getIntercomStatusDisplay();
    QLineEdit *getIntercomRecvStatusDisplay();
    QPushButton *getIntercomDisconnectButton();

    // Floor
    ToggleButton *getUpButton();
    ToggleButton *getDownButton();
    QCheckBox *getFireCheckbox();
    QCheckBox *getPowerCheckbox();
    QLineEdit *getFireStatusDisplay();
    QComboBox *getFloorSelector();

    // Render
    void update();
    void reset();

private:
    MainWindow *mainWindow;

    // Elevator
    ToggleButton* destinationButtons[NUM_FLOORS];

    // Floor
    ToggleButton* upButton;
    ToggleButton* downButton;
};

#endif // UIELEMENTS_H
