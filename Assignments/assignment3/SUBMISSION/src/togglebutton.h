#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QPushButton>

class ToggleButton : public QPushButton
{
    Q_OBJECT

public:
    // OPTIONS
    const static int NoDisable = 0;
    const static int AutoDisable = 1;

    ToggleButton(const QString &text = "", int shouldAutoDisable = ToggleButton::NoDisable, QWidget *parent = nullptr);
    ~ToggleButton();

public slots:
    void toggle();
    void setOn(bool isOn);

private:
    bool isOn;
    bool shouldAutoDisable; // automatically disables button on click (programmatically re-enable afterwards)
};

#endif // TOGGLEBUTTON_H
