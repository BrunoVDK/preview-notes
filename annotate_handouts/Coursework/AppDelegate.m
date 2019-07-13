//
//  AppDelegate.m
//  coursework
//
//  Created by Bruno Vandekerkhove on 13/07/2019.
//  Copyright © 2019 Bruno Vandekerkhove. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return true;
}

- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender {
    return false;
}

@end
