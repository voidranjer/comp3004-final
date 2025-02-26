#include "battery.h"

Battery::Battery(QObject *parent, QList<QLabel *> elements, NeuresetController* controller)
    : QObject{parent}, elements(elements), controller(controller)
{
    outOfBattery = false;

    batteryLife = BATTERY_LIFE;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){ tickTime(); });
    timer->start(CLOCK_TICK);

    batteryAnnouncement = 90;
}

bool Battery::getOutOfBattery()
{
    return outOfBattery;
}

void Battery::reduceBattery()
{
    if (elements[0]->styleSheet().contains("green")) {
        elements[0]->setStyleSheet("background-color: white; border: 1px solid black;");
        return;
    }

    if (elements[1]->styleSheet().contains("green")) {
        elements[1]->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0;");
        return;
    }
    if (elements[2]->styleSheet().contains("green")) {
        elements[2]->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0; border-top: 0;");
        return;
    }
    if (elements[3]->styleSheet().contains("green")) {
        elements[3]->setStyleSheet("background-color: white; border: 1px solid black; border-top: 0;");
        elements[4]->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0;");
        elements[5]->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0; border-top: 0;");
        elements[6]->setStyleSheet("background-color: yellow; border: 1px solid black; border-top: 0;");
        elements[7]->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0;");
        elements[8]->setStyleSheet("background-color: yellow; border: 1px solid black; border-bottom: 0; border-top: 0;");
        elements[9]->setStyleSheet("background-color: yellow; border: 1px solid black; border-top: 0;");
        return;
    }

    if (elements[4]->styleSheet().contains("yellow")) {
        elements[4]->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0;");
        return;
    }
    if (elements[5]->styleSheet().contains("yellow")) {
        elements[5]->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0; border-top: 0; border-bottom: 0; border-top: 0;");
        return;
    }
    if (elements[6]->styleSheet().contains("yellow")) {
        elements[6]->setStyleSheet("background-color: white; border: 1px solid black; border-top: 0;");
        elements[7]->setStyleSheet("background-color: red; border: 1px solid black; border-bottom: 0;");
        elements[8]->setStyleSheet("background-color: red; border: 1px solid black; border-bottom: 0; border-top: 0;");
        elements[9]->setStyleSheet("background-color: red; border: 1px solid black; border-top: 0;");
        return;
    }

    if (elements[7]->styleSheet().contains("red")) {
        elements[7]->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0;");
        return;
    }
    if (elements[8]->styleSheet().contains("red")) {
        elements[8]->setStyleSheet("background-color: white; border: 1px solid black; border-bottom: 0; border-top: 0; border-bottom: 0; border-top: 0;");
        return;
    }
    if (elements[9]->styleSheet().contains("red")) {
        elements[9]->setStyleSheet("background-color: white; border: 1px solid black; border-top: 0;");
    }

    outOfBattery = true;
}

void Battery::flashBatteries()
{
    qDebug() << "Out of Battery!";

    for (QWidget* element : elements) {
        element->show();
    }

    QTimer::singleShot(500, this, [=]() {
        for (QWidget* element : elements) {
            element->hide();
        }

        QTimer::singleShot(500, this, [=]() {
            for (QWidget* element : elements) {
                element->show();
            }

            QTimer::singleShot(650, this, [=]() {
                for (QWidget* element : elements) {
                    element->hide();
                }
            });
        });
    });

}


void Battery::tickTime()
{
    if (!controller->getIsOn()) {
        return;
    }

    if (outOfBattery) {
        qDebug() << "Battery Life is now 0!";
        qDebug() << "Device is now off: Please charge the device!";
        controller->turnOff();
        return;
    }

    if (controller->getInSession()) {
        --batteryLife;
    }

    --batteryLife;

    int batteryPercentage = (batteryLife * 100) / BATTERY_LIFE;

    if (batteryPercentage <= batteryAnnouncement && !outOfBattery) {
        reduceBattery();
        if (batteryAnnouncement == 20){
            qDebug() << "Battery life now at" << batteryAnnouncement << "%";
            qDebug() << "Battery life LOW please plug in device";
            controller->lowBattery();
        }
        else if (batteryAnnouncement == 10){
            qDebug() << "Battery life now at" << batteryAnnouncement << "%";
            qDebug() << "Battery life CRITICALLY LOW low please plug in device";
        }
        else{
            qDebug() << "Battery life now at" << batteryAnnouncement << "%";
        }
        batteryAnnouncement -= 10;

    }


}

