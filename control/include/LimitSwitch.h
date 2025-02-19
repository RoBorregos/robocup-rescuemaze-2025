#ifndef LimitSwitchh
#define LimitSwitch_h

#include "Arduino.h"

namespace LimitSwitchID {
    constexpr uint8_t kLeft = 0;
    constexpr uint8_t kRight = 1;
};

class LimitSwitch {
    private:
        bool state_;
        uint8_t id_;
        uint8_t pin_;

    public:
        LimitSwitch();
        void initLimitSwitch(const uint8_t pin);
        bool getState();
        void printState();
        void LimitSwitchActive();
        void initLimitSwitchInternal();
};

#endif