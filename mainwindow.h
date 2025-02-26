#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <qcustomplot.h>
#include "pcwindow.h"
#include "eegsimulator.h"
#include "neuresetcontroller.h"

class NeuresetController;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<QWidget *> static createBattery();
    bool getInSession();
    bool getIsOn();
    void tickTime();

public slots:
    void powerButtonClicked();
    void breakContact();
    void startSession();
    void endSession();
    void handleElectrodeSelection(int index);
    void pauseSession();
    void resumeSession();
    void addLogEntry(QString entry);
    void lowBattery();

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    bool isOn = true; // will go to false when program is setting up
    void changeRedLight();
    void changeMachineState();
    void giveTreatment();
    void init();
    PCWindow *pcWindow;
    EEGSimulator *eegSimulator;
    NeuresetController *controller;
    int sessionTimer;
    QTimer *timer = nullptr;
    QTimer *contactTimer = nullptr;
    int contactCount;
    void countInactivity();
    QTimer *pauseTimer;
    int pauseCount;
};
#endif // MAINWINDOW_H
