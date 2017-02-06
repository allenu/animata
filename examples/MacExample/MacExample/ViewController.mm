//
//  ViewController.m
//  MacExample
//
//  Created by Allen Ussher on 2/5/17.
//  Copyright © 2017 Allen Ussher. All rights reserved.
//

#import "ViewController.h"
#import "animata.h"
#import "ActorDescriptionFromJsonDocument.h"

@implementation ViewController {
    NSTimer *_timer;
    
    BOOL _punchButtonPressed;
    BOOL _kickButtonPressed;
    
    ActorDescription _actorDescription;
    ActorState _playerActorState;
    ActorState _enemyActorState;
    
    IBOutlet NSImageView *_playerImageView;
    IBOutlet NSImageView *_enemyImageView;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"SimpleExample" ofType:@"json"];
    if (filePath != nil) {
        NSString *jsonString = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:NULL];
        
        const char *utf_string = [jsonString UTF8String];
        std::string stl_str = std::string(utf_string);
        
        _actorDescription = ActorDescriptionFromJsonDocument(stl_str);
    }
    
    _playerActorState = { _actorDescription.default_state, 0 };
    _enemyActorState = { _actorDescription.default_state, 0 };
    
    // Make enemy image horizontally flipped.
    [_enemyImageView scaleUnitSquareToSize:NSMakeSize(-1.0, 1.0)];
    
    [self startTimer];
}

- (void)viewWillDisappear {
    [self stopTimer];
    [super viewWillDisappear];
}

- (IBAction)didPressPunch:(id)sender {
    _punchButtonPressed = YES;
}

- (IBAction)didPressKick:(id)sender {
    _kickButtonPressed = YES;
}

- (void)startTimer {
    _timer = [NSTimer scheduledTimerWithTimeInterval:1/5.0
                                              target:self
                                            selector:@selector(timerTick:)
                                            userInfo:nil
                                             repeats:YES];

}

- (void)stopTimer {
    [_timer invalidate];
}

- (void)timerTick:(NSTimer *)timer {
    std::vector<std::string> player_directions;
    std::vector<std::string> enemy_directions;
    
    if (_punchButtonPressed) {
        player_directions.push_back("punch");
    }
    if (_kickButtonPressed) {
        player_directions.push_back("kick");
    }
    _punchButtonPressed = NO;
    _kickButtonPressed = NO;
    
    const AnimationState & playerAnimationState = _actorDescription.states[_playerActorState.state_name];
    const PropertySet & previousAnimationStateFrame = playerAnimationState.frames[_playerActorState.frame_index];
    if (previousAnimationStateFrame.IntWithName("attack") > 0) {
        enemy_directions.push_back("hurt");
    }
    
    _playerActorState = NextActorState(_actorDescription, _playerActorState, player_directions);
    _enemyActorState = NextActorState(_actorDescription, _enemyActorState, enemy_directions);
    
    [self updateImages];
}

- (NSImage *)imageForStateName:(std::string)stateName frameIndex:(int)frameIndex {
    const AnimationState & animationState = _actorDescription.states[stateName];
    const PropertySet & previousAnimationStateFrame = animationState.frames[frameIndex];
    const char *sprite = previousAnimationStateFrame.StringWithName("sprite").c_str();

    NSString *imageName = [[NSString alloc] initWithCString:sprite encoding:kUnicodeUTF8Format];
    NSImage *image = [NSImage imageNamed:imageName];
    return image;
}

- (void)updateImages {
    NSImage *playerImage = [self imageForStateName:_playerActorState.state_name frameIndex:_playerActorState.frame_index];
    [_playerImageView setImage:playerImage];

    NSImage *enemyImage = [self imageForStateName:_enemyActorState.state_name frameIndex:_enemyActorState.frame_index];
    [_enemyImageView setImage:enemyImage];
}

@end
