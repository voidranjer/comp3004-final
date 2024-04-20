#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QMainWindow>
#include "sessionlogger.h"

namespace Ui {
class PCWindow;
}

class PCWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PCWindow(QWidget *parent, SessionLogger *logger);
    ~PCWindow();
public slots:
    void syncChanges();
private:
    Ui::PCWindow *ui;
    SessionLogger *logger;
};

#endif // PCWINDOW_H
