#include "pcwindow.h"
#include "ui_pcwindow.h"

PCWindow::PCWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PCWindow)
{
    ui->setupUi(this);
}

PCWindow::~PCWindow()
{
    delete ui;
}
