
#pragma once

#include <string>
#include "ActorDescription.h"
#include "ActorState.h"

ActorState NextActorState(const ActorDescription & actorDescription,
			  const ActorState & previousActorState, 
			  const std::vector<std::string> & directions);
