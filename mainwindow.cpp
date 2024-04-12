#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->powerButton, SIGNAL(released()), this, SLOT (powerButtonClicked()));
    connect(ui->break_contact, SIGNAL(released()), this, SLOT (breakContact()));
    connect(ui->start_session, SIGNAL(released()), this, SLOT (startSession()));

    changeMachineState(); // to hide the objects that shouldn't be visible when the device is powered off
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMachineState()
{
    isOn = !isOn;
    QList<QWidget*> elements = {ui->battery1, ui->battery2, ui->battery3, ui->battery_top, ui->break_contact, ui->start_session, ui->past_session, ui->change_date};

    for (QWidget* element : elements) {
        if (isOn) {
            element->show();
        } else {
            element->hide();
        }
    }

    if (!isOn) {
        ui->start_session->setEnabled(false);

        if (inContact) {
            breakContact();
        }
    }
}

void MainWindow::reduceBattery()
{
    if (ui->battery1->styleSheet().contains("green")) {
        ui->battery1->setStyleSheet("background-color: white; border: 1px solid black;");
        ui->battery2->setStyleSheet("background-color: yellow; border: 1px solid black;");
        ui->battery3->setStyleSheet("background-color: yellow; border: 1px solid black;");
        return;
    }
    if (ui->battery2->styleSheet().contains("yellow")) {
        ui->battery2->setStyleSheet("background-color: white; border: 1px solid black;");
        ui->battery3->setStyleSheet("background-color: red; border: 1px solid black;");
        return;
    }
    if (ui->battery3->styleSheet().contains("red")) {
        ui->battery3->setStyleSheet("background-color: white; border: 1px solid black;");
    }
    outOfBattery = true;
}

void MainWindow::giveTreatment()
{
    ui->green_light->setStyleSheet("background-color: green;");

    QTimer::singleShot(1500, [=]() {
        ui->green_light->setStyleSheet("background-color: white; border: 3px solid green;");
    });

    // Add other logic for changes calculations and whatnot if needed (to show difference after treatment)
}

void MainWindow::powerButtonClicked()
{
    if (outOfBattery) {
        flashBatteries();

        return;
    }

    QPushButton* button = ui->powerButton;

    if (button->palette().color(QPalette::ButtonText) == Qt::red) {
        button->setStyleSheet("QPushButton {color: blue; border: none;}");
        changeMachineState();
    } else {
        button->setStyleSheet("QPushButton {color: red; border: none;}");
        changeMachineState();
    }
}

void MainWindow::breakContact() {
    inContact = !inContact;

    if (inContact) {
        ui->blue_light->setStyleSheet("background-color: blue;");
        ui->break_contact->setText("Break Contact");
        ui->start_session->setEnabled(true);
    } else {
        ui->blue_light->setStyleSheet("background-color: white; border: 3px solid blue;");
        ui->break_contact->setText("Make Contact");
        ui->start_session->setEnabled(false);
        if (inSession) {
            loopChangeRedLight();
        }
    }
}

void MainWindow::loopChangeRedLight() {
    if (!inSession || inContact || !isOn) {
        return;
    }
    changeRedLight();

    QTimer::singleShot(2000, this, &MainWindow::loopChangeRedLight);
}

void MainWindow::flashBatteries()
{
    ui->battery_top->show();
    ui->battery1->show();
    ui->battery2->show();
    ui->battery3->show();

    QTimer::singleShot(1000, this, [=]() {
        ui->battery_top->hide();
        ui->battery1->hide();
        ui->battery2->hide();
        ui->battery3->hide();

        QTimer::singleShot(500, this, [=]() {
            ui->battery_top->show();
            ui->battery1->show();
            ui->battery2->show();
            ui->battery3->show();

            QTimer::singleShot(1000, this, [=]() {
                ui->battery_top->hide();
                ui->battery1->hide();
                ui->battery2->hide();
                ui->battery3->hide();
            });
        });
    });
}


void MainWindow::changeRedLight() {
    ui->red_light->setStyleSheet("background-color: red;");

    QTimer::singleShot(1000, [=]() {
        ui->red_light->setStyleSheet("background-color: white; border: 3px solid red;");
    });
}

void MainWindow::startSession()
{
    if (outOfBattery) {
        powerButtonClicked();
        return;
    }
    inSession = true;
    QList<QWidget*> elements = {ui->start_session, ui->past_session, ui->change_date};

    for (QWidget* element : elements) {
        element->hide();

    }
}

