
#pragma once

#include <string>
#include <vector>
#include "PropertySet.h"

struct AnimationStateTransition {
	std::string from;
    std::vector<std::string> allowed_directions;
    std::string next_state;
    PropertySet properties;
};
