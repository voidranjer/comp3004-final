#ifndef ELEVATORFLOORSLIDER_H
#define ELEVATORFLOORSLIDER_H

#include <QSlider>
#include <QGridLayout>
#include <QString>

class ElevatorFloorSlider : public QSlider
{
    Q_OBJECT

public:
    ElevatorFloorSlider(QWidget *parent = nullptr);

};

#endif // ELEVATORFLOORSLIDER_H
