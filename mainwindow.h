#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <qcustomplot.h>
#include "pcwindow.h"
#include <eegsimulator.h>
#include "neuresetcontroller.h"

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

public slots:
    void powerButtonClicked();
    void breakContact();
    void startSession();
    void endSession();
    void handleElectrodeSelection(int index);
    void pauseSession();
    void resumeSession();

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    bool isOn = true; // will go to false when program is setting up
    void changeRedLight();
    void loopChangeRedLight();
    void changeMachineState();
    void giveTreatment();
    void init();
    PCWindow *pcWindow;
    EEGSimulator *eegSimulator;
    NeuresetController *controller;
};
#endif // MAINWINDOW_H
