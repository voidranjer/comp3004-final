#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QMainWindow>

namespace Ui {
class PCWindow;
}

class PCWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PCWindow(QWidget *parent = nullptr);
    ~PCWindow();

private:
    Ui::PCWindow *ui;
};

#endif // PCWINDOW_H
