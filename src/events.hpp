#pragma once

#include "evening.hpp"
#include "vector.hpp"

namespace event {

extern evening::Channel bus;

struct LeftPlayerMove {
    Vector<double> position;
};

struct RightPlayerMove {
    Vector<double> position;
};

struct BallMove {
    Vector<double> position;
};

} // namespace event
