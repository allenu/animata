
#include <assert.h>
#include <algorithm>
#include <type_traits>
#include "ActorStateMachine.h"

std::vector<std::string> FilterGroupsThatIncludeState(std::map<std::string, AnimationStateGroup> groups, std::string state_name) {
	std::vector<std::string> foundGroupNames;
	for (auto it = groups.begin(); it != groups.end(); ++it) {
		std::string groupName = it->first;
		const std::vector<std::string> & states = it->second.states;
		
		if (find(states.begin(), states.end(), state_name) != states.end()) {
			foundGroupNames.push_back(groupName);
		}
	}
	return foundGroupNames;
}

bool StringVectorsHaveCommonElements(std::vector<std::string> first_vector, std::vector<std::string> second_vector) {
	for (auto firstItem : first_vector) {
		if (find(second_vector.begin(), second_vector.end(), firstItem) != second_vector.end()) {
			return true;
		}
	}
	return false;
}

ActorState NextActorState(const ActorDescription & actorDescription,
			  const ActorState & previousActorState, 
			  const std::vector<std::string> & directions) {
	// By default, carry over the previous state.
	ActorState actorState = previousActorState;
	
	// Get groups previousActorState.state_name is in.
	auto groups = FilterGroupsThatIncludeState(actorDescription.stateGroups, previousActorState.state_name);
	
	auto previousAnimationStateIt = actorDescription.states.find(previousActorState.state_name);
	const AnimationState *previousAnimationState = nullptr;
	if (previousAnimationStateIt != actorDescription.states.end()) {
		previousAnimationState = &previousAnimationStateIt->second;
	} else {
		// We couldn't find that state by name!
		assert(false);
	}
	
	// Find matching transition.
		// Matching transition requires (groups_we_are_in.includes(from) or from == previousActorState.state_name)
		// and allowed_inputs includes directions.
	AnimationStateTransition matchingTransition;
	bool foundTransition = false;
	for (auto transition : actorDescription.transitions) {
		bool isAnyState = transition.from == "any";
		bool matchesGroupName = find(groups.begin(), groups.end(), transition.from) != groups.end();
		bool matchesStateName = transition.from == previousActorState.state_name;
		bool directionsMatch = StringVectorsHaveCommonElements(transition.allowed_directions, directions);
		
		if ((isAnyState || matchesGroupName || matchesStateName) && directionsMatch) {
			matchingTransition = transition;
			foundTransition = true;
			break;
		}
	}
	
	// If found matching transition.
		// state_name = transition.next_state
		// if transition.properties.do_not_reset_frame_index, frame_index = previous.frame_index + 1, if new frame_index >= new_state.frames.size(), frame_index = 0
		// else frame_index = 0
	if (foundTransition) {
		auto animationStateIt = actorDescription.states.find(matchingTransition.next_state);
		if (animationStateIt != actorDescription.states.end()) {
			actorState.state_name = matchingTransition.next_state;
			
			const AnimationState & animationState = animationStateIt->second;
			if (matchingTransition.properties.BoolWithName("do_not_reset_frame_index")) {
				actorState.frame_index = previousActorState.frame_index + 1;
				if (actorState.frame_index >= animationState.frames.size()) {
					actorState.frame_index = 0;
				}
			} else {
				actorState.frame_index = 0;
			}
		} else {
			// Couldn't find that state, so go to default instead...
			actorState.state_name = actorDescription.default_state;
			actorState.frame_index = 0;
		}
	}
	
	// No matching transition? Move onto the next frame in the frame sequence.
		// If already on last frame, go to whatever is next_state and reset frame_index.
		// If no next_state, then go to "default" state with frame_index=0.
	else {
		actorState.frame_index = previousActorState.frame_index + 1;
		if (actorState.frame_index >= previousAnimationState->frames.size()) {
			auto next_state_name = previousAnimationState->properties.StringWithName("next");
			if ( next_state_name != "") {
				actorState.state_name = next_state_name;
			} else {
				actorState.state_name = actorDescription.default_state;
			}
			actorState.frame_index = 0;
		}
	}
	
	return actorState;
}
