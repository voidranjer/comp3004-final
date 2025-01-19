#ifndef BUTTON_H
#define BUTTON_H

class Button {
   public:
    void on();
    void off();
    void disable();
    void enable();

   private:
    bool isActivated;
    bool isDisabled;
};

#endif