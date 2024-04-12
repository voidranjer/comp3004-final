#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>


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
    void changeMachineState();
    void reduceBattery();

public slots:
    void powerButtonClicked();
    void breakContact();

private:
    Ui::MainWindow *ui;
    bool outOfBattery = false;
    bool isOn = true; // will go to false when program is setting up
    bool inContact = false;
};
#endif // MAINWINDOW_H
