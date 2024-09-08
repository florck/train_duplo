//
// Created by Florian Thöni on 08/09/2024.
//
#include <cstdint>
#include "lib_train_4_emilien_native.h"


int16_t convertPotPositionToSpeed(const int16_t position) {
    // Linear transformation outside the center range
    // doc says that it’s between -100 and 100, but indeed it’s 80 max…
    const double normalized = (position - 2047.) / 2047.;
    const double speed = normalized * normalized * normalized * 77.0;

    return static_cast<int16_t>(speed);
}
