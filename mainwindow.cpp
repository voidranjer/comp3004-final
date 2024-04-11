#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->powerButton, SIGNAL(released()), this, SLOT (powerButtonClicked()));
    changeMachineState(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMachineState(bool isOn)
{
    QList<QLabel*> elements = {ui->battery1, ui->battery2, ui->battery3, ui->battery_top};

    for (QLabel* element : elements) {
        if (isOn) {
            element->show();
        } else {
            element->hide();
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

void MainWindow::powerButtonClicked()
{
    QPushButton* button = ui->powerButton;

    if (button->palette().color(QPalette::ButtonText) == Qt::red) {
        button->setStyleSheet("QPushButton {color: blue; border: none;}");
        changeMachineState(false);
    } else {
        button->setStyleSheet("QPushButton {color: red; border: none;}");
        changeMachineState(true);
    }

//    reduceBattery();
}

