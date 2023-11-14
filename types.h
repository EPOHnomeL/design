#ifndef TYPES_H
#define TYPES_H

#include <QString>

enum States {
    Idle,
    EditProfile,
    ChooseProfile,
    Start,
    Mixing
};

struct Profile{
    uint16_t profileID, time, rpm, percentWater;
};

#endif // TYPES_H
