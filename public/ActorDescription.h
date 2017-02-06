
#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>
#include <tuple>
#include "AnimationState.h"
#include "AnimationStateGroup.h"
#include "AnimationStateTransition.h"
#include "PropertySet.h"

struct ActorDescription {
    // Note that states and groups share the same namespace.
    std::map<std::string, AnimationState> states;			// state name -> state
    std::map<std::string, AnimationStateGroup> stateGroups;	// group name -> state group

    std::vector<AnimationStateTransition> transitions;
	
	std::string default_state;

    PropertySet propertySet;

    // Simple optimizations to save us from looking up whether a name is a state name or a group name.
    std::set<std::string> state_names;
    std::set<std::string> group_names;
};
