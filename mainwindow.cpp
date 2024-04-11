#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->powerButton, SIGNAL(released()), this, SLOT (clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clicked()
{
    QPushButton* button = ui->powerButton;

    if (button->palette().color(QPalette::ButtonText) == Qt::red) {
        button->setStyleSheet("QPushButton {color: blue; border: none;}");
    } else {
        button->setStyleSheet("QPushButton {color: red; border: none;}");
    }
}
