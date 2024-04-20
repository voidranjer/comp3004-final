#include "pcwindow.h"
#include "ui_pcwindow.h"

PCWindow::PCWindow(QWidget *parent, SessionLogger *logger)
    : QMainWindow(parent)
    , ui(new Ui::PCWindow), logger(logger)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::released, this, &PCWindow::syncChanges);
}

PCWindow::~PCWindow()
{
    delete ui;
}

void PCWindow::syncChanges()
{
    // clear list
    ui->listWidget->clear();

    QVector<LogEntry*> entries = logger->getLogEntries();

    // add entries to list
    for (LogEntry* entry : entries)
    {
        ui->listWidget->addItem(entry->getLogText());
    }
}
