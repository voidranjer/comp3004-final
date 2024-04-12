#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->powerButton, SIGNAL(released()), this, SLOT (powerButtonClicked()));
    connect(ui->break_contact, SIGNAL(released()), this, SLOT (breakContact()));

    changeMachineState(); // to hide the objects that shouldn't be visible when the device is powered off
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMachineState()
{
    isOn = !isOn;
    QList<QWidget*> elements = {ui->battery1, ui->battery2, ui->battery3, ui->battery_top, ui->break_contact};

    for (QWidget* element : elements) {
        if (isOn) {
            element->show();
        } else {
            element->hide();
        }
    }

    if (!isOn && inContact) {
        breakContact();
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

void MainWindow::powerButtonClicked()
{
    QPushButton* button = ui->powerButton;

    if (button->palette().color(QPalette::ButtonText) == Qt::red) {
        button->setStyleSheet("QPushButton {color: blue; border: none;}");
        changeMachineState();
    } else {
        button->setStyleSheet("QPushButton {color: red; border: none;}");
        changeMachineState();
    }
}

void MainWindow::breakContact()
{
    inContact = !inContact;

    if (inContact) {
        ui->blue_light->setStyleSheet("background-color: blue;");
        ui->break_contact->setText("Break Contact");
    } else {
        ui->blue_light->setStyleSheet("background-color: white; border: 3px solid blue;");
        ui->break_contact->setText("Make Contact");
    }
}

