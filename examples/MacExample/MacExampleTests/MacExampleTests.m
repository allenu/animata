//
//  MacExampleTests.m
//  MacExampleTests
//
//  Created by Allen Ussher on 2/5/17.
//  Copyright © 2017 Ussher Press. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <string>
#import "PropertySetFromJsonDictionary.h"
#import "ActorDescriptionFromJsonDocument.h"
#import "ActorStateMachine.h"

@interface MacExampleTests : XCTestCase

@end

@implementation MacExampleTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testEmptyPropertySet {
    NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"PropertySet_Empty" ofType:@"json"];
    if (filePath != nil) {
        NSString *jsonString = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:NULL];
        
        const char *utf_string = [jsonString UTF8String];
        std::string stl_str = std::string(utf_string);
        
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse( stl_str, root );
        if (parsingSuccessful) {
            PropertySet propertySet = PropertySetFromJsonDictionary(root);
            XCTAssert(propertySet.properties.size() == 0);
        }
    } else {
        XCTFail();
    }
}

- (void)testNormalPropertySet {
    NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"PropertySet_Normal" ofType:@"json"];
    if (filePath != nil) {
        NSString *jsonString = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:NULL];
        
        const char *utf_string = [jsonString UTF8String];
        std::string stl_str = std::string(utf_string);
        
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse( stl_str, root );
        if (parsingSuccessful) {
            PropertySet propertySet = PropertySetFromJsonDictionary(root);
            XCTAssert(propertySet.properties.size() == 3);
            XCTAssert(propertySet.IntWithName("int value") == 1234);
            XCTAssert(propertySet.DoubleWithName("double value") == 3.14);
            XCTAssert(propertySet.StringWithName("string value") == "some string value here");
        }
    } else {
        XCTFail();
    }
}

- (ActorDescription)actorDescriptionFromFilename:(NSString *)filename {
    ActorDescription actorDescription;
    
    NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:filename ofType:@"json"];
    if (filePath != nil) {
        NSString *jsonString = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:NULL];
        
        const char *utf_string = [jsonString UTF8String];
        std::string stl_str = std::string(utf_string);
        
        actorDescription = ActorDescriptionFromJsonDocument(stl_str);
    } else {
        XCTFail();
    }
    
    return actorDescription;
}

- (void)testNormalActorDescription {
    ActorDescription actorDescription = [self actorDescriptionFromFilename:@"SampleActorDescription"];
}

- (void)runActorDescription:(ActorDescription)actorDescription withInput:(std::string)input atFrameIndex:(int)frameIndex forFrames:(int)frames {
    const ActorState startState = { actorDescription.default_state };
    ActorState state = startState;
    for (int i=0; i < frames; ++i) {
        ActorState previousState = state;
        std::vector<std::string> input_directions = {};
        if (i == frameIndex) {
            input_directions.push_back(input);
            NSLog(@"input: %s at frame %d\n",  input.c_str(), i);
        }
        state = NextActorState(actorDescription, previousState, input_directions);
        
        const char *previousStateName = previousState.state_name.c_str();
        const char *stateName = state.state_name.c_str();
        
        const AnimationState & previousAnimationState = actorDescription.states[previousStateName];
        const PropertySet & previousAnimationStateFrame = previousAnimationState.frames[previousState.frame_index];
        const char *previousSprite = previousAnimationStateFrame.StringWithName("sprite").c_str();
        
        const AnimationState & animationState = actorDescription.states[stateName];
        const PropertySet & animationStateFrame = animationState.frames[state.frame_index];
        const char *sprite = animationStateFrame.StringWithName("sprite").c_str();
        
        NSLog(@"%s / %d / %s => %s / %d / %s\n", previousStateName, previousState.frame_index, previousSprite,
              stateName, state.frame_index, sprite);
    }
}

- (void)testSimpleExample {
    ActorDescription actorDescription = [self actorDescriptionFromFilename:@"SimpleExample"];
    
    NSLog(@"Loop without inputs");
    [self runActorDescription: actorDescription withInput:"none" atFrameIndex:0 forFrames: 10];
    
    NSLog(@"\n\nLoop with 'punch' at 4");
    [self runActorDescription: actorDescription withInput:"punch" atFrameIndex:5 forFrames: 10];
}


- (void)testThreeStateActor {
    ActorDescription actorDescription = [self actorDescriptionFromFilename:@"ThreeStateActor"];
    
    NSLog(@"Loop without inputs");
    [self runActorDescription: actorDescription withInput:"none" atFrameIndex:0 forFrames: 10];
    
    NSLog(@"\n\nLoop with 'second' as input");
    [self runActorDescription: actorDescription withInput:"second" atFrameIndex:5 forFrames: 10];
}

- (void)testGroups {
    ActorDescription actorDescription = [self actorDescriptionFromFilename:@"Groups"];
    
    NSLog(@"Loop without inputs");
    [self runActorDescription: actorDescription withInput:"none" atFrameIndex:0 forFrames: 20];
    
    NSLog(@"\n\nLoop with 'fruit_goal' at 5");
    [self runActorDescription: actorDescription withInput:"fruit_goal" atFrameIndex:5 forFrames: 20];
    
    NSLog(@"\n\nLoop with 'color_goal' at 5");
    [self runActorDescription: actorDescription withInput:"color_goal" atFrameIndex:5 forFrames: 20];
    
    NSLog(@"\n\nLoop with 'fruit_goal' at 8");
    [self runActorDescription: actorDescription withInput:"fruit_goal" atFrameIndex:8 forFrames: 20];
    
    NSLog(@"\n\nLoop with 'color_goal' at 8");
    [self runActorDescription: actorDescription withInput:"color_goal" atFrameIndex:8 forFrames: 20];
}

- (void)testAnyGroup {
    ActorDescription actorDescription = [self actorDescriptionFromFilename:@"Groups"];
    
    NSLog(@"Loop without inputs");
    [self runActorDescription: actorDescription withInput:"none" atFrameIndex:0 forFrames: 20];
    
    NSLog(@"\n\nLoop with 'any_goal' at 5");
    [self runActorDescription: actorDescription withInput:"any_goal" atFrameIndex:5 forFrames: 20];
}

- (void)testNoResetOnTransition {
    ActorDescription actorDescription = [self actorDescriptionFromFilename:@"TestNoResetOnTransition"];
    
    NSLog(@"Loop without inputs");
    [self runActorDescription: actorDescription withInput:"none" atFrameIndex:0 forFrames: 20];
    
    NSLog(@"\n\nLoop with 'goal' at 5");
    [self runActorDescription: actorDescription withInput:"goal" atFrameIndex:5 forFrames: 20];
}

@end
