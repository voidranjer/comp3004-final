#include "togglebutton.h"

#define OFF_STYLES  "background-color: #7c818c; border: 1px solid black; border-radius: 5px; padding: 5px;"
#define ON_STYLES   "background-color: #5294e2; border: 1px solid black; border-radius: 5px; padding: 5px;"

ToggleButton::ToggleButton(const QString &text, int shouldAutoDisable, QWidget *parent) : QPushButton(text, parent), shouldAutoDisable(shouldAutoDisable), isOn(false)
{
    setStyleSheet(OFF_STYLES);
    connect(this, &QPushButton::clicked, this, &ToggleButton::toggle);
}

ToggleButton::~ToggleButton()
{

}

void ToggleButton::toggle()
{
    setOn(!isOn);

    if (shouldAutoDisable == ToggleButton::AutoDisable) setEnabled(false);
}

void ToggleButton::setOn(bool isOn)
{
    this->isOn = isOn;
    setStyleSheet(isOn ? ON_STYLES : OFF_STYLES);
}
