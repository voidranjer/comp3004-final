#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <qcustomplot.h>
#include "pcwindow.h"
#include <eegsimulator.h>

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

public slots:
    void powerButtonClicked();
    void breakContact();
    void startSession();
    void endSession();
    // void updateEEGPlot();
    void handleElectrodeSelection(int index);

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    bool outOfBattery = false;
    bool isOn = true; // will go to false when program is setting up
    void changeRedLight();
    void loopChangeRedLight();
    void flashBatteries();
    void reduceBattery();
    void changeMachineState();
    void giveTreatment();
    PCWindow *pcWindow;
    EEGSimulator *eegSimulator;
};
#endif // MAINWINDOW_H
