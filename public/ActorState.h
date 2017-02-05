
#pragma once

#include "PropertySet.h"

struct ActorState {
	std::string state_name;
	int frame_index;
	bool facing_left;
	
    PropertySet properties;
};
