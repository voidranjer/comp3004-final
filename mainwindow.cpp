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
    // init components
    QList<QLabel *> elements = {ui->battery_top,   ui->battery1,
                                 ui->battery1_2,    ui->battery1_3,
                                 ui->battery2,      ui->battery2_2,
                                 ui->battery2_3,    ui->battery3,
                                 ui->battery3_2,    ui->battery3_3
    };

    controller = new NeuresetController(this, elements, this);
    eegSimulator = new EEGSimulator(this, ui->customPlot);

    // connect buttons
    connect(ui->powerButton, SIGNAL(released()), this, SLOT (powerButtonClicked()));
    connect(ui->break_contact, SIGNAL(released()), this, SLOT (breakContact()));
    connect(ui->start_session, SIGNAL(released()), this, SLOT (startSession()));
    connect(ui->end_session, SIGNAL(released()), this, SLOT (endSession()));
    connect(ui->pause_session, SIGNAL(released()), this, SLOT (pauseSession()));
    connect(ui->resume_session, SIGNAL(released()), this, SLOT (resumeSession()));

    connect(eegSimulator, &EEGSimulator::administerFeedback, this, [this](){giveTreatment();});

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

    // LOGGER: Event handler
    connect(eegSimulator, &EEGSimulator::sessionCompleted, controller, &NeuresetController::handleSessionCompleted);
}

void MainWindow::countInactivity()
{
    ++pauseCount;
    if (pauseCount >= INACTIVITY_TIME) {
        qDebug() << "Due to inactivity, the session has been stopped.";
        pauseTimer->stop();
        endSession();
    }
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
                                  ui->battery3_3,    ui->datetimeDisplay,
                                  ui->start_session

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
    ui->pause_session->hide();
    ui->resume_session->hide();
    ui->session_progress->hide();
    ui->session_timer->hide();

    if (!isOn) {
        ui->start_session->setEnabled(false);

        if (eegSimulator->getInContact()) {
            breakContact();
        }
    }
}

bool MainWindow::getInSession() {
    return eegSimulator->getInSession();
}

bool MainWindow::getIsOn()
{
    return isOn;
}

void MainWindow::giveTreatment()
{
    ui->green_light->setStyleSheet("background-color: green;");

    QTimer::singleShot(1000, [=]() {
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
        eegSimulator->continueTreatment();

        if (timer != nullptr) {
            timer->start();
        }
    } else {
        ui->blue_light->setStyleSheet("background-color: white; border: 3px solid blue;");
        ui->break_contact->setText("Make Contact");
        ui->start_session->setEnabled(false);
        eegSimulator->pauseTreatment();
        qDebug() << "You have 5 seconds to reconnect contact with patient.";
        QTimer::singleShot(5000, [=]() {
            if (!eegSimulator->getInContact()) {
                endSession();
            }
        });
        if (timer != nullptr) {
            timer->stop();
        }

        if (inSession) {
            contactCount = 0;
            contactTimer = new QTimer(this);
            connect(contactTimer, &QTimer::timeout, this, [this](){ changeRedLight(); });
            contactTimer->start(CLOCK_TICK);

        }
    }
}

void MainWindow::changeRedLight() {
    if (!eegSimulator->getInSession() || eegSimulator->getInContact() || !isOn) {
        contactTimer->stop();
        return;
    }

    ++contactCount;

    if (contactCount >= INACTIVITY_TIME) {
        qDebug() << "Due to inactivity, the session has been stopped.";
        contactTimer->stop();
        endSession();
    }

    ui->red_light->setStyleSheet("background-color: red;");

    QTimer::singleShot(500, [=]() {
        ui->red_light->setStyleSheet("background-color: white; border: 3px solid red;");
    });
}

void MainWindow::startSession()
{
    ui->session_progress->setValue(0);
    if (controller->getOutOfBattery()) {
        powerButtonClicked();
        return;
    }

    ui->tabs->setTabEnabled(1, false);
    ui->tabs->setTabEnabled(2, false);

    if (eegSimulator) {
        eegSimulator->startSession();
    }

    ui->start_session->hide();
    ui->end_session->show();
    ui->pause_session->show();
    ui->resume_session->hide();
    ui->session_progress->show();
    ui->session_timer->show();

    if (timer) {
        timer->stop();
    }

    ui->session_timer->setText("0:29");
    sessionTimer = 29;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){ tickTime(); });
    timer->start(CLOCK_TICK);
}

void MainWindow::tickTime()
{
    if (sessionTimer == 0) {
        timer->stop();
        endSession();
        return;
    }

    --sessionTimer;

    QString formattedTimer = QString("0:%1").arg(sessionTimer, 2, 10, QLatin1Char('0'));
    ui->session_timer->setText(formattedTimer);

    int progress = (29 - sessionTimer) * 100 / 29;  // Assuming sessionTimer starts at 29
    ui->session_progress->setValue(progress);
}

void MainWindow::endSession()
{
    eegSimulator->endSession();

    ui->tabs->setTabEnabled(1, true);
    ui->tabs->setTabEnabled(2, true);

    ui->start_session->show();

    ui->end_session->hide();
    ui->pause_session->hide();
    ui->resume_session->hide();
    ui->session_progress->hide();
    ui->session_timer->hide();
}

void MainWindow::pauseSession()
{
    ui->pause_session->hide();
    ui->resume_session->show();
    timer->stop();

    pauseCount = 0;
    pauseTimer = new QTimer(this);
    connect(pauseTimer, &QTimer::timeout, this, [this](){ countInactivity(); });
    eegSimulator->pauseTreatment();
    pauseTimer->start(CLOCK_TICK);
}

void MainWindow::resumeSession()
{
    ui->pause_session->show();
    ui->resume_session->hide();
    timer->start();
    eegSimulator->continueTreatment();
    pauseTimer->stop();
}
