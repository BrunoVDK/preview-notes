//
//  AppDelegate.m
//  extract_notes
//
//  Created by Bruno Vandekerkhove on 17/11/18.
//  Copyright (c) 2018 Bruno Vandekerkhove. All rights reserved.
//

#import "AppDelegate.h"

#import <Quartz/Quartz.h>

@interface AppDelegate ()
@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return true;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    [textview setString:@"Drag & drop PDF on the icon (or open it with the file menu)"];
}

- (IBAction)openDocument:(id)sender {
    
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseFiles:YES];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:YES];
    [panel setAllowedFileTypes:[NSArray arrayWithObject:@"pdf"]];
    [panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton) {
            if (panel.URLs.count > 0)
                [self application:sender openFile:((NSURL *)panel.URLs.firstObject).path];
        }
    }];
    
}

- (void)application:(NSApplication *)sender openFiles:(NSArray *)filenames {
    if (filenames.count > 0)
        [self application:sender openFile:filenames.firstObject];
}

- (BOOL)application:(NSApplication *)sender openFile:(NSString *)filename {
    
    NSData *data = [NSData dataWithContentsOfFile:filename];
    PDFDocument *document = [[PDFDocument alloc] initWithData:data];
    NSString *annotations_text = @"", *annotation_text, *text;
    for (int i=0 ; i<document.pageCount ; i++) {
        PDFPage *page = [document pageAtIndex:i];
        for (PDFAnnotation *annotation in [page annotations]) {
            text = [annotation contents];
            if (text.length > 0 && [annotation isMemberOfClass:[PDFAnnotationPopup class]]) {
                annotation_text = [NSString stringWithFormat:@"(Page %i)\n%@\n---\n", i, text];
                annotations_text = [annotations_text stringByAppendingString:annotation_text];
            }
        }
    }
    
    [textview setString:annotations_text];

    return true;
    
}

@end