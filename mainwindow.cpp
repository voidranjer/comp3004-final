#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <neuresetcontroller.h>
#include <QTimer>
#include "eegsimulator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Show pc window
    pcWindow = new PCWindow(this);
    pcWindow->show();

    connect(ui->powerButton, SIGNAL(released()), this, SLOT (powerButtonClicked()));
    connect(ui->break_contact, SIGNAL(released()), this, SLOT (breakContact()));
    connect(ui->start_session, SIGNAL(released()), this, SLOT (startSession()));
    connect(ui->end_session, SIGNAL(released()), this, SLOT (endSession()));

    changeMachineState(); // to hide the objects that shouldn't be visible when the device is powered off

    NeuresetController *controller = new NeuresetController(this); // todo: move to header file

    // CLOCK: Tick the clock
    connect(controller, &NeuresetController::timeChanged, ui->datetimeDisplay, [this](QDateTime datetime){ ui->datetimeDisplay->setText(datetime.toString("yyyy-MM-dd hh:mm:ss"));});

    // CLOCK: Update the time picker when first enabling the clock settings page
    connect(ui->tabs, &QTabWidget::currentChanged, ui->dateTimeEdit,
            [this, controller](int tabIndex) {
              // if (tabIndex != 2) return; (trying to not hard code for now, at the cost of efficiency)
              ui->dateTimeEdit->setDateTime(controller->getDatetime());
            });

    // CLOCK: TODO - aekus
    // connect(ui->change_date, &QPushButton::released, controller, &NeuresetController::toggleClockSetting);
    // connect(controller, &NeuresetController::clockSettingActiveChanged,
    //         ui->clockSettings, [this](bool active) {
    //             if (active) {
    //                 ui->clockSettings->show();
    //             } else {
    //                 ui->clockSettings->hide();
    //             }
    //         });

    // CLOCK: Event handler for set_time button
    connect(ui->set_time, &QPushButton::released, controller, [this, controller](){
        controller->setDatetime(ui->dateTimeEdit->dateTime());
    });

    // setting initial statis of graph
    ui->customPlot->xAxis->setRange(0, 4000);
    ui->customPlot->yAxis->setRange(-700, 700);
    ui->customPlot->xAxis->setTickLabels(false);
    ui->customPlot->xAxis->setTicks(false);
    ui->customPlot->yAxis->setTickLabels(false);
    ui->customPlot->yAxis->setTicks(false);

    // add electrodes
    ui->comboBox->addItem("Electrode 1");
    ui->comboBox->addItem("Electrode 2");
    ui->comboBox->addItem("Electrode 3");
    ui->comboBox->addItem("Electrode 4");
    ui->comboBox->addItem("Electrode 5");
    ui->comboBox->addItem("Electrode 6");
    ui->comboBox->addItem("Electrode 7");

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleElectrodeSelection);
}

MainWindow::~MainWindow()
{
    delete eegSimulator;
    delete ui;
}

void MainWindow::handleElectrodeSelection(int index) {
    if (eegSimulator) {
        eegSimulator->selectElectrode(index);
    } else {
        qDebug() << "EEG simulator is not initialized.";
    }
}


void MainWindow::changeMachineState()
{
    isOn = !isOn;
     QList<QWidget *> elements = {ui->battery1,      ui->battery2,
                                  ui->battery3,      ui->battery_top,
                                  ui->neuresetBox,   ui->battery1_2,
                                  ui->battery1_3,    ui->battery2_2,
                                  ui->battery2_3,    ui->battery3_2,
                                  ui->battery3_3
     };

     for (QWidget* element : elements) {
         if (isOn) {
             element->show();
         } else {
             element->hide();
         }
     }

    if (inSession) {
        inSession = false;
        reduceBattery();
    }

    ui->end_session->hide();

    if (!isOn) {
        ui->start_session->setEnabled(false);

        if (inContact) {
            breakContact();
        }
    }
}

void MainWindow::reduceBattery()
{
    if (ui->battery_top->styleSheet().contains("green")) {
        ui->battery_top->setStyleSheet("background-color: white; border: 1px solid black;");
        return;
    }

    if (ui->battery1->styleSheet().contains("green")) {
        ui->battery1->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0;");
        return;
    }
    if (ui->battery1_2->styleSheet().contains("green")) {
        ui->battery1_2->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0; border-top: 0;");
        return;
    }
    if (ui->battery1_3->styleSheet().contains("green")) {
        ui->battery1_3->setStyleSheet("background-color: white; border: 1px solid black; border-top: 0;");
        ui->battery2->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0;");
        ui->battery2_2->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0; border-top: 0;");
        ui->battery2_3->setStyleSheet("background-color: yellow; border: 1px solid black; border-top: 0;");
        ui->battery3->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0;");
        ui->battery3_2->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0; border-top: 0;");
        ui->battery3_3->setStyleSheet("background-color: yellow; border: 1px solid black; border-top: 0;");
        return;
    }

    if (ui->battery2->styleSheet().contains("yellow")) {
        ui->battery2->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0;");
        return;
    }
    if (ui->battery2_2->styleSheet().contains("yellow")) {
        ui->battery2_2->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0; border-top: 0; border-bottom: 0; border-top: 0;");
        return;
    }
    if (ui->battery2_3->styleSheet().contains("yellow")) {
        ui->battery2_3->setStyleSheet("background-color: white; border: 1px solid black; border-top: 0;");
        ui->battery3->setStyleSheet("background-color: red; border: 1px solid black; border-bottom: 0;");
        ui->battery3_2->setStyleSheet("background-color: red; border: 1px solid black; border-bottom: 0; border-top: 0;");
        ui->battery3_3->setStyleSheet("background-color: red; border: 1px solid black; border-top: 0;");
        return;
    }

    if (ui->battery3->styleSheet().contains("red")) {
        ui->battery3->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0;");
        return;
    }
    if (ui->battery3_2->styleSheet().contains("red")) {
        ui->battery3_2->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0; border-top: 0; border-bottom: 0; border-top: 0;");
        return;
    }
    if (ui->battery3_3->styleSheet().contains("red")) {
        ui->battery3_3->setStyleSheet("background-color: white; border: 1px solid black; border-top: 0;");
    }

    outOfBattery = true;
}

void MainWindow::giveTreatment()
{
    ui->green_light->setStyleSheet("background-color: green;");

    QTimer::singleShot(1500, [=]() {
        ui->green_light->setStyleSheet("background-color: white; border: 3px solid green;");
    });
}

void MainWindow::powerButtonClicked()
{
    if (outOfBattery) {
        flashBatteries();
        ui->blue_light->setStyleSheet("background-color: white; border: 3px solid blue;");

        if (!isOn) {
            return;
        }
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
    if (outOfBattery) {
        powerButtonClicked();
        return;
    }

    inContact = !inContact;

    if (inContact) {
        ui->blue_light->setStyleSheet("background-color: blue;");
        ui->break_contact->setText("Break Contact");
        ui->start_session->setEnabled(true);

        eegSimulator = new EEGSimulator(ui->customPlot, 7, this);

    } else {
        ui->blue_light->setStyleSheet("background-color: white; border: 3px solid blue;");
        ui->break_contact->setText("Make Contact");
        if (eegSimulator) {
            delete eegSimulator;
            eegSimulator = nullptr;
        }
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
    QList<QWidget *> elements = {ui->battery_top,   ui->battery1,
                                 ui->battery1_2,    ui->battery1_3,
                                 ui->battery2,      ui->battery2_2,
                                 ui->battery2_3,    ui->battery3,
                                 ui->battery3_2,    ui->battery3_3
    };

    for (QWidget* element : elements) {
        element->show();
    }

    QTimer::singleShot(500, this, [=]() {
        for (QWidget* element : elements) {
            element->hide();
        }

        QTimer::singleShot(500, this, [=]() {
            for (QWidget* element : elements) {
                element->show();
            }

            QTimer::singleShot(650, this, [=]() {
                for (QWidget* element : elements) {
                    element->hide();
                }
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
    if (eegSimulator) {
        eegSimulator->startTreatment();
    }
    for (QWidget* element : elements) {
        element->hide();
    }

    ui->end_session->show();
}

void MainWindow::endSession()
{
    inSession = false;
    reduceBattery();

    QList<QWidget*> elements = {ui->start_session, ui->past_session, ui->change_date};

    for (QWidget* element : elements) {
        element->show();
    }

    ui->end_session->hide();
}

