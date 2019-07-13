//
//  Document.h
//  coursework
//
//  Created by Bruno Vandekerkhove on 13/07/2019.
//  Copyright © 2019 Bruno Vandekerkhove. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

@interface Document : NSDocument<PDFViewDelegate, NSTextViewDelegate> {
    PDFDocument *document;
    PDFAnnotation *annotation;
}

- (IBAction)firstSlide:(id)sender;
- (IBAction)lastSlide:(id)sender;
- (IBAction)previousSlide:(id)sender;
- (IBAction)nextSlide:(id)sender;

@end

