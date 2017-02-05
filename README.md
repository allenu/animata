# Animata

Animata is a simple finite state machine library for use in 2D sprite-based games, written in C++.

This is an work in progress.


## Installation

For a quick example, clone the repository with --recursive ([JsonCpp](https://github.com/open-source-parsers/jsoncpp) is used), then:

```
make test
```

Take a look at examples/console/main.cpp.


## Usage

First, define your ActorDescription in a JSON file (more details below). For readability, here is an example first written in YAML. You can use a [YAML to JSON script](https://gist.github.com/xiaohanyu/67df9c2b958ac8fc0cf1) to convert this.

```yaml
states:
  standing:
    frames:
      - sprite: standing1
      - sprite: standing2
      - sprite: standing3
    default_state: true
    next: standing
  punching:
    frames:
      - sprite: punching1
        attack: 10
      - sprite: punching2
  kicking:
    frames:
      - sprite: kicking1
        attack: 2
      - sprite: kicking2
  hurt:
    frames:
      - sprite: hurt1
      - sprite: hurt2
    uninterruptible: true
groups:
  attacking: 
    - punching
    - kicking
transitions:
  - from: any
    excluding: 
      - hurt
      - attacking
    to: punching
    input:
      - punch
  - from: any
    excluding: hurt
    to: kicking
    input:
      - kick
  - from: any
    excluding: hurt
    to: hurt
    input:
      - hurt
```

In your code, you would load the JSON file as a string and call ActorDescriptionFromJsonDocument() with it to
get an ActorDescription object.

The API for Animata is functional. The workflow for its use in a game engine would be as follows:

- Initialize ActorState if this is the first frame, or use the previous frame's ActorState.
- Read user inputs
- Convert user inputs into "directions", which are simply strings defining the desired state change
- Call NextActorState() with the ActorDescription, the previous ActorState, and the directions.
- Store the output of NextActorState as the new current ActorState.
- Use the ActorState's state_frame and frame_index to look up the current AnimationState frame to display.
- Using the properties of the AnimationState frame (a PropertySet), display your user sprite.

Note that Animata does not take care of the details of player positioning, physics, rendering, or game rules.

TODO: A diagram and details explaining how Animata would fit into a game engine alongside the above.


```
#include <animata.h>

...

// Load the ActorDescription
std::string jsonString = "..."; // Obtain a JSON from a file
ActorDescription actorDescription = ActorDescriptionFromJsonDocument(jsonString);

...

const ActorState initialState = { actorDescription.default_state, 0 };
const std::vector<string> inputs = { "punch" };

const ActorState nextState = NextActorState(initialState, initialState, inputs);
const AnimationState & nextAnimationState = actorDescription.states[nextState.state_name];
const PropertySet & animationFrame = nextAnimationState.frames[nextState.frame_index];

printf("New state: %s, frame index: %d, sprite name: %s\n", nextState.state_name, nextState.frame_index, animationFrame.stringWithName("sprite"));

```

## JSON Format

A deserializer is provided to create an ActorDescription from a JSON file, but of course you can create the
ActorDescription in whatever manner you'd like. If you use the provided code to read from a JSON, you'd simply
provide ActorDescriptionFromJsonDocument() a JSON object encoded in a std::string and it will return an ActorDescription.

At the top level, these key-value pairs can be defined:
- states
- groups
- transitions

### states

These define the states in the finite state machine describing your actor. At a minimum, a state should have one or more
frames. Each frame is simply a dictionary of key-value pairs, defined as a PropertySet. It is up to the client to
define what the contents of a frame are, but typically they would at least contain a reference to a sprite image.
Often, frames would include many other attributes, such as the attack value if the state is an attacking one, or
a displacement if the frame is a movement one.

```yaml
states:
  standing:
    frames:
      - sprite: standing1
      - sprite: standing2
      - sprite: standing3
    default_state: true
    next: standing
  punching:
    frames:
      - sprite: punching1
        attack: 10
      - sprite: punching2
```

### groups

States can be grouped and given a name. This serves only for conveniently defining transitions. The groups
section is a dictionary and contains zero or more groups. Each value in the dictionary is simply a list
of strings, each string referencing a state by name.

Group names and state names share the same namespace, so a group's must be unique relative to all other groups
and all state names.

```yaml
groups:
  attacking: 
    - punching
    - kicking
```

### transitions

Transitions define how the current state can be transformed to a new state given 1. the current state and 2. an
input.

The transitions section is simply a list of dictionaries. Each dictionary defines a from-to mapping given
an input state and input direction. At runtime, this list is consulted from top to bottom and the first
match found where the current state name (or group) matches the 'from' field and the 'input' section includes
one or more of the input directions and the 'excluding' section does not include the current state (or group).
If not all criteria are met for a given match, the runtime algorithm moves to the next item in the list to
find a match. As such, the items in transition list must be in priority order.

The 'from' and 'excluding' fields can reference either a state or a group of states.


```yaml
transitions:
  - from: any
    excluding:
      - hurt
      - attacking
    to: punching
    input:
      - punch
  - from: any
    excluding: hurt
    to: kicking
    input:
      - kick
  - from: any
    excluding: hurt
    to: hurt
    input:
      - hurt
```


## PropertySet

Animata uses PropertySet to 


## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

2017-01-05: First published on github.

## Credits

Animata makes use of [JsonCpp](https://github.com/open-source-parsers/jsoncpp).

## License

Animata is licensed under the MIT license. Please see LICENSE.txt for more details.

If you use this library, please provide a link to this github page somewhere in your app and give proper credit to the author.
