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
    void changeMachineState(bool isOn);
    void reduceBattery();

public slots:
    void powerButtonClicked();

private:
    Ui::MainWindow *ui;
    bool outOfBattery = false;
};
#endif // MAINWINDOW_H
