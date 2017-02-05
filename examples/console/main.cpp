
#include <string>
#include <stdio.h>
#include <animata.h>
#include <ActorDescriptionFromJsonDocument.h>

int main(int argc, char *argv[]) {
    const std::string jsonDocument = "{\"states\":{\"standing\":{\"frames\":[{\"sprite\":\"standing1\"},{\"sprite\":\"standing2\"},{\"sprite\":\"standing3\"}],\"default_state\":true,\"next\":\"standing\"},\"punching\":{\"frames\":[{\"sprite\":\"punching1\",\"attack\":10},{\"sprite\":\"punching2\"}]},\"kicking\":{\"frames\":[{\"sprite\":\"kicking1\",\"attack\":2},{\"sprite\":\"kicking2\"}]},\"hurt\":{\"frames\":[{\"sprite\":\"hurt1\"},{\"sprite\":\"hurt2\"}],\"uninterruptible\":true}},\"groups\":{\"attacking\":[\"punching\",\"kicking\"]},\"transitions\":[{\"from\":\"any\",\"excluding\":[\"hurt\",\"attacking\"],\"to\":\"punching\",\"input\":[\"punch\"]},{\"from\":\"any\",\"excluding\":\"hurt\",\"to\":\"kicking\",\"input\":[\"kick\"]},{\"from\":\"any\",\"excluding\":\"hurt\",\"to\":\"hurt\",\"input\":[\"hurt\"]}]}";

    ActorDescription actorDescription = ActorDescriptionFromJsonDocument(jsonDocument);

    const ActorState initialState = { actorDescription.default_state, 0 };
//    const std::vector<std::string> inputs = { "punch" };
    const std::vector<std::string> inputs = { "none" };

    const ActorState nextState = NextActorState(actorDescription, initialState, inputs);
    const AnimationState & nextAnimationState = actorDescription.states[nextState.state_name];
    const PropertySet & animationFrame = nextAnimationState.frames[nextState.frame_index];

    printf("New state: %s, frame index: %d, sprite name: %s\n", nextState.state_name.c_str(), nextState.frame_index, animationFrame.StringWithName("sprite").c_str());

    // Loop a bunch of times on it
    printf("\n----\n");
    ActorState currentState = nextState;
    for (int i=0; i < 20; ++i) {
	currentState = NextActorState(actorDescription, currentState, inputs);

	const ActorState nextState = NextActorState(actorDescription, currentState, inputs);
	const AnimationState & nextAnimationState = actorDescription.states[currentState.state_name];
	const PropertySet & animationFrame = nextAnimationState.frames[currentState.frame_index];
	
	printf("New state: %s, frame index: %d, sprite name: %s\n", currentState.state_name.c_str(), currentState.frame_index, animationFrame.StringWithName("sprite").c_str());
    }
    
    return 0;
}
