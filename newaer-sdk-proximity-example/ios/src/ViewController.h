//
//  ViewController.h
//  newaer-sdk-proximity-example
//
//  Copyright (c) 2013 NewAer. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <NAPlatformDelegate, UITextViewDelegate,UITableViewDelegate> {
  NSString *lastLogMsg;
}

@property (weak, nonatomic) IBOutlet UITextView *textView;

@property (weak, nonatomic) IBOutlet UISwitch *advertiseNewAerSwitch;

@property (weak, nonatomic) IBOutlet UISwitch *scanNewAerSwitch;
@property (weak, nonatomic) IBOutlet UISwitch *scanWearableSwitch;
@property (weak, nonatomic) IBOutlet UISwitch *scanBeaconSwitch;

@property (weak, nonatomic) IBOutlet UISegmentedControl *logSegment;
@property (strong) NSMutableDictionary *devices;
@property (weak, nonatomic) IBOutlet UITableView *devicesTableView;
@property (weak, nonatomic) IBOutlet UIButton *sendButton;
@property (copy) NSIndexPath *selectedIndexPath;

@property (assign) BOOL visible;
@property (weak) NADevice *devicePendingMessage;

-(NSString *)formatLogMsgWithString:(NSString*)string;

- (IBAction) didChangeWearableSwitch;
- (IBAction) didChangeNewAerSwitch;
- (IBAction) didChangeBeaconSwitch;

- (IBAction) didChangeAdvertiseNewAerSwitch;

- (IBAction) didClickSendMessage:(id)sender;
- (IBAction) didChangeLogSegment;

@end
