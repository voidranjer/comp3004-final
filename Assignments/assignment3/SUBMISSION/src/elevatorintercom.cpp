#include "elevatorintercom.h"
#include <QDebug>

ElevatorIntercom::ElevatorIntercom() {}

void ElevatorIntercom::blinkIndicator()
{
    qInfo() << "Indicator blinking!";
}

void ElevatorIntercom::establishVoice()
{
    qInfo() << "Voice established!";
}
