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

    // initialize all components
    init();
}

void MainWindow::init()
{
    QList<QLabel *> elements = {ui->battery_top,   ui->battery1,
                                 ui->battery1_2,    ui->battery1_3,
                                 ui->battery2,      ui->battery2_2,
                                 ui->battery2_3,    ui->battery3,
                                 ui->battery3_2,    ui->battery3_3
    };

    // init components
    controller = new NeuresetController(this, elements);
    eegSimulator = new EEGSimulator(this, ui->customPlot);

    // connect buttons
    connect(ui->powerButton, SIGNAL(released()), this, SLOT (powerButtonClicked()));
    connect(ui->break_contact, SIGNAL(released()), this, SLOT (breakContact()));
    connect(ui->start_session, SIGNAL(released()), this, SLOT (startSession()));
    connect(ui->end_session, SIGNAL(released()), this, SLOT (endSession()));

    // to hide the objects that shouldn't be visible when the device is powered off
    changeMachineState();

    // CLOCK: Tick the clock
    connect(controller, &NeuresetController::timeChanged, ui->datetimeDisplay, [this](QDateTime datetime){ ui->datetimeDisplay->setText(datetime.toString("yyyy-MM-dd hh:mm:ss"));});

    // CLOCK: Update the time picker when first enabling the clock settings page
    connect(ui->tabs, &QTabWidget::currentChanged, ui->dateTimeEdit,
            [=](int tabIndex) {
              // if (tabIndex != 2) return; (trying to not hard code for now, at the cost of efficiency)
              ui->dateTimeEdit->setDateTime(controller->getDatetime());
            });

    // CLOCK: Event handler for set_time button
    connect(ui->set_time, &QPushButton::released, controller, [=](){
        controller->setDatetime(ui->dateTimeEdit->dateTime());
    });

    // GRAPH: setting initial statis of graph
    ui->customPlot->xAxis->setRange(0, 4000);
    ui->customPlot->yAxis->setRange(-700, 700);
    ui->customPlot->xAxis->setTickLabels(false);
    ui->customPlot->xAxis->setTicks(false);
    ui->customPlot->yAxis->setTickLabels(false);
    ui->customPlot->yAxis->setTicks(false);

    // GRAPH: add electrodes
    ui->comboBox->addItem("Electrode 1");
    ui->comboBox->addItem("Electrode 2");
    ui->comboBox->addItem("Electrode 3");
    ui->comboBox->addItem("Electrode 4");
    ui->comboBox->addItem("Electrode 5");
    ui->comboBox->addItem("Electrode 6");
    ui->comboBox->addItem("Electrode 7");
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleElectrodeSelection);

    // CLOCK: Tick the clock
    connect(controller, &NeuresetController::timeChanged, ui->datetimeDisplay, [this](QDateTime datetime){ ui->datetimeDisplay->setText(datetime.toString("yyyy-MM-dd hh:mm:ss"));});
}


MainWindow::~MainWindow()
{
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

    if (eegSimulator->getInSession()) {
        eegSimulator->endSession();
    }

    ui->end_session->hide();

    if (!isOn) {
        ui->start_session->setEnabled(false);

        if (eegSimulator->getInContact()) {
            breakContact();
        }
    }
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
    if (controller->getOutOfBattery()) {
        ui->blue_light->setStyleSheet("background-color: white; border: 3px solid blue;");
        controller->flashBatteries();

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
    if (controller->getOutOfBattery()) {
        powerButtonClicked();
        return;
    }

    bool inContact = eegSimulator->toggleContact();
    bool inSession = eegSimulator->getInSession();

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
    if (!eegSimulator->getInSession() || eegSimulator->getInContact() || !isOn) {
        return;
    }
    changeRedLight();

    QTimer::singleShot(2000, this, &MainWindow::loopChangeRedLight);
}

void MainWindow::changeRedLight() {
    ui->red_light->setStyleSheet("background-color: red;");

    QTimer::singleShot(1000, [=]() {
        ui->red_light->setStyleSheet("background-color: white; border: 3px solid red;");
    });
}

void MainWindow::startSession()
{
    if (controller->getOutOfBattery()) {
        powerButtonClicked();
        return;
    }

    QList<QWidget*> elements = {ui->start_session, ui->past_session, ui->change_date};
    if (eegSimulator) {
        eegSimulator->startSession();
    }
    for (QWidget* element : elements) {
        element->hide();
    }

    ui->end_session->show();
}

void MainWindow::endSession()
{
    eegSimulator->endSession();

    QList<QWidget*> elements = {ui->start_session, ui->past_session, ui->change_date};

    for (QWidget* element : elements) {
        element->show();
    }

    ui->end_session->hide();
}

