//
//  ActorDescriptionFromJsonDocument.cpp
//  Y2K
//
//  Created by Allen Ussher on 2/5/17.
//  Copyright © 2017 Allen Ussher. All rights reserved.
//


#include <assert.h>
#include "ActorDescription.h"
#include "PropertySetFromJsonDictionary.h"
#include "json.h"

ActorDescription ActorDescriptionFromJsonDocument(const std::string & document) {
	ActorDescription actorDescription;
	
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( document, root );
	if (parsingSuccessful)
	{
		std::vector<std::string> special_state_property_names = {
			"frames"
		};
		std::vector<std::string> special_transition_property_names = {
			"from",
			"input",
			"to",
			"excluding"
		};
		
		// Load states
		const Json::Value states = root["states"];
		std::string first_state_name = "";
		Json::Value::Members stateNames( states.getMemberNames() );
		for ( Json::Value::Members::iterator it = stateNames.begin();
			 it != stateNames.end();
			 ++it )
		{
			const std::string & name = *it;
			const Json::Value state = states[name];
			
			if (first_state_name == "") {
				first_state_name = name;
			}
			
			// Make sure it hasn't been used yet as a name.
			assert(actorDescription.state_names.find(name) == actorDescription.state_names.end());
			
			AnimationState animationState;
			
			animationState.properties = PropertySetFromJsonDictionary(state, special_state_property_names);
			
			const Json::Value frames = state["frames"];
			for (int frameNum=0; frameNum < frames.size(); frameNum++) {
				Json::Value frame = state["frames"][frameNum];
				PropertySet animationFrame = PropertySetFromJsonDictionary(frame);
				
				animationState.frames.push_back(animationFrame);
			}

			actorDescription.states[name] = animationState;
			actorDescription.state_names.insert(name);
			
			if (animationState.properties.BoolWithName("default_state")) {
				actorDescription.default_state = name;
			}
		}
		
		// Must be supplied!
		assert(actorDescription.default_state != "");
		
		// Load groups
		const Json::Value groups = root["groups"];
		Json::Value::Members groupNames( groups.getMemberNames() );
		for ( Json::Value::Members::iterator groupNameIt = groupNames.begin();
			 groupNameIt != groupNames.end();
			 ++groupNameIt ) {
			const std::string & groupName = *groupNameIt;
			const Json::Value stateNameArray = groups[groupName];
			
			// Make sure it hasn't been used yet as a state name or a group name.
			assert(actorDescription.state_names.find(groupName) == actorDescription.state_names.end());
			assert(actorDescription.group_names.find(groupName) == actorDescription.group_names.end());

			AnimationStateGroup animationStateGroup;
			for (int stateNum=0; stateNum < stateNameArray.size(); stateNum++) {
				animationStateGroup.states.push_back( stateNameArray[stateNum].asString() );
			}
			actorDescription.stateGroups[groupName] = animationStateGroup;
			actorDescription.group_names.insert(groupName);
		}
		
		// Load transitions
		const Json::Value transitions = root["transitions"];
		for (int transitionNum=0; transitionNum < transitions.size(); transitionNum++) {
			AnimationStateTransition transition;
			
			const Json::Value transitionEntry = transitions[transitionNum];
			
			transition.from = transitionEntry["from"].asString();
			
			const Json::Value inputDirections = transitionEntry["input"];
			for (int directionNum = 0; directionNum < inputDirections.size(); directionNum++) {
				transition.allowed_directions.push_back( inputDirections[ directionNum ].asString() );
			}
				
			transition.next_state = transitionEntry["to"].asString();

			transition.properties = PropertySetFromJsonDictionary(transitionEntry, special_transition_property_names);
			
			assert(transition.from != "");
			assert(transition.next_state != "");
			actorDescription.transitions.push_back(transition);
		}
	}
	
	return actorDescription;
}
