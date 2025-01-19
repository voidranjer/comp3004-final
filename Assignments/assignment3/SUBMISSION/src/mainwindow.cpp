#include <QListWidget>
#include<QDebug>
#include <QComboBox>
#include <QTimer>

#include "mainwindow.h"
#include "defs.h"
#include "ecs.h"
#include "floor.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Singleton
    UIElements *uiElements = new UIElements(this);
    Floor *floor = new Floor(uiElements); // PLEASE FIX THISSSSSSS THIS IS WRONG
    ECS *ecs = ECS::instance(uiElements);

    // Initialize elevator selector
    QComboBox *elevatorSelector = ui->elevatorSelector;
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        elevatorSelector->addItem(QString::number(i + 1));
    }
    connect(elevatorSelector, &QComboBox::currentTextChanged, ecs, &ECS::handleElevatorChange);

    // Initialize reset simulation button
    QPushButton *resetButton = ui->resetButton;
    connect(resetButton, &QPushButton::clicked, ecs, &ECS::reset);

    // Main event loop (TODO: MEMORY MANAGEMENT)
    QTimer *masterTimer = new QTimer(this);
    connect(masterTimer, &QTimer::timeout, ecs, &ECS::updateTick);
    masterTimer->start(TIMER_TICK_DELAY);

}

MainWindow::~MainWindow() {
    ECS::destroyInstance();
    delete ui;
}

