//
//  PixelatedImageView.m
//  MacExample
//
//  Created by Allen Ussher on 2/5/17.
//  Copyright © 2017 Ussher Press. All rights reserved.
//

#import "PixelatedImageView.h"

@implementation PixelatedImageView

- (void)drawRect:(NSRect)dirtyRect {
    [[NSGraphicsContext currentContext] setImageInterpolation: NSImageInterpolationNone];
    [super drawRect:dirtyRect];
}

@end
