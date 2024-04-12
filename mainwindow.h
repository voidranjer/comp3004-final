#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QThread>

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

private:
    Ui::MainWindow *ui;
    bool outOfBattery = false;
    bool isOn = true; // will go to false when program is setting up
    bool inContact = false;
    bool inSession = false;
    void changeRedLight();
    void loopChangeRedLight();
    void flashBatteries();
    void reduceBattery();
    void changeMachineState();
    void giveTreatment();

};
#endif // MAINWINDOW_H
