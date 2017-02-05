
#pragma once

#include <string>
#include <vector>
#include "PropertySet.h"

struct AnimationState {
    std::vector<PropertySet> frames;
    PropertySet properties;
    // - next_state
};
