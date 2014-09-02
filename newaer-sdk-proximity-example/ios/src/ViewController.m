//
//  ViewController.m
//  newaer-sdk-proximity-example
//
//  Copyright (c) 2013 NewAer. All rights reserved.
//

#import "NAPlatform.h"
#import "ViewController.h"
#import "DevicetableCustomCell.h"

@interface ViewController ()
@property (strong) NSTimer *updateTimer;
@end

@implementation ViewController

- (void)viewDidLoad
{
  [super viewDidLoad];
  self.visible = NO;
  
  self.updateTimer = [NSTimer timerWithTimeInterval:1.0f target:self selector:@selector(updateTimerInterval:) userInfo:nil repeats:YES];
  [[NSRunLoop currentRunLoop] addTimer:self.updateTimer forMode:NSDefaultRunLoopMode];
  
	// Do any additional setup after loading the view, typically from a nib.
  [NAPlatform setLogLevel:NALOGLEVEL_DEBUG classes:nil logger:^(NALogLevel level, NSString *message) {
    NSLog(@"[NAPlatform] %@",message);
  }];
  [NAPlatform initializeWithApplicationId:@"adf40370-4ad3-4796-b617-1542888c3b81" delegate:self];
  [[NAPlatform sharedNAPlatform]
   registerFriendApplicationIds: @[@"cefcb4ce-03dd-466e-ac06-7c13117c323e",
                                   @"88cae6eb-55f5-4c03-bd36-6ccb50b0afba"]];
  
  [[NAPlatform sharedNAPlatform] registerBeaconIds:@[ @"2a6b70e5-f974-4d42-911e-d44eff379b95" ]];

  self.scanWearableSwitch.on = NO;
  self.scanNewAerSwitch.on = NO;
  self.scanBeaconSwitch.on = NO;
  self.advertiseNewAerSwitch.on = NO;
  self.devices = [[NSMutableDictionary alloc] init];

  [self didChangeWearableSwitch];
  [self didChangeNewAerSwitch];
  [self didChangeBeaconSwitch];
  [self didChangeAdvertiseNewAerSwitch];
  [self setupForegroundBackground];
}

- (void)viewDidAppear:(BOOL)animated
{
  self.visible = YES;
}

- (BOOL) prefersStatusBarHidden
{
  return YES;
}

- (void)didReceiveMemoryWarning
{
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}


- (void)setupForegroundBackground
{
  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(applicationDidEnterBackground:)
                                               name:UIApplicationDidEnterBackgroundNotification
                                             object:nil];
  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(applicationWillEnterForeground:)
                                               name:UIApplicationWillEnterForegroundNotification
                                             object:nil];
}

-(void)applicationDidEnterBackground:(NSNotification *)note
{
  NSLog(@"EXAMPLE: didEnterBackground");
}

-(void)applicationWillEnterForeground:(NSNotification *)note
{
  NSLog(@"EXAMPLE: didEnterForeground");
}


#pragma mark Delegates
   
- (void)registrationFailed:(NARegistrationFailureType)error
{
  NSLog(@"registrationFailed: %d",error);
}
   
- (void)platformDidFindDevice:(NADevice*)device
{
  [self.devices setObject:device forKey:device.deviceId];
  
  dispatch_async(dispatch_get_main_queue(),^{
    [self.devicesTableView reloadData];
    self.textView.text = [self.textView.text stringByAppendingString:
                     [self formatLogMsgWithString:[NSString stringWithFormat:@"+ [%@] %@",[device deviceTypeAsString], device.name]]];
    [self.textView scrollRangeToVisible:NSMakeRange(self.textView.text.length - 1, 1)];
  });
}

//
// Called when |NAPlatform| loses contact with a previously discovered device.
//
- (void)platformDidLoseDevice:(NADevice*)device
{
  @synchronized(self.devices) {
    if([self.devices objectForKey:device.deviceId] != nil) {
      [self.devices removeObjectForKey:device.deviceId];
    }
  }
  
  dispatch_async(dispatch_get_main_queue(),^{
     self.textView.text = [self.textView.text stringByAppendingString:
                        [self formatLogMsgWithString:[NSString stringWithFormat:@"- [%@] %@",[device deviceTypeAsString], device.name]]];
    [self.textView scrollRangeToVisible:NSMakeRange(self.textView.text.length - 1, 1)];
    
    [self.devicesTableView reloadData];
  });
}

- (void)platformDidUpdateDevice:(NADevice *)device
{
  NSLog(@"platformDidUpdateDevice: %@",device);
  @synchronized(self.devices) {
    [self.devices setObject:device forKey:device.deviceId];
  }
  dispatch_async(dispatch_get_main_queue(),^{
    [self.devicesTableView reloadData];
  });
}

- (void)platformDeviceHealthDidChange:(NADevice*)device
{
  return;
}

- (void)platformDidReceiveMessage:(NAMessage *)message from:(NADevice *)device
{
  dispatch_sync(dispatch_get_main_queue(),^{
    NSString *messageString = [[NSString alloc] initWithData:message.data encoding:NSUTF8StringEncoding];
    self.textView.text = [self.textView.text stringByAppendingString:
                     [self formatLogMsgWithString:[NSString stringWithFormat:@"M %@ <- %@",device.deviceId,messageString]]];
    [self.textView scrollRangeToVisible:NSMakeRange(self.textView.text.length - 1, 1)];
  });
}

- (void)platformScanFailed:(NADeviceType)type
{
  NSString *message = @"startScanWithDeviceTypes failed, make sure Bluetooth is enabled.";
  NSLog(@"%@",message);
  dispatch_async(dispatch_get_main_queue(),^{
    self.textView.text = [self.textView.text stringByAppendingString:
                          [self formatLogMsgWithString:message]];
    [self.textView scrollRangeToVisible:NSMakeRange(self.textView.text.length - 1, 1)];
  });
}

- (void)platformDidCompleteOneScan:(NADeviceType)type
{
  
}

- (void)platformDidStartScan:(NADeviceType)type {
  
}

- (void)platformDidStopScan:(NADeviceType)type {
  
}

- (void)platformAdvertiseFailed:(NADeviceType)type {
  
}

-(NSString *)formatLogMsgWithString:(NSString*)string
{
  if (![string hasSuffix: @"\n"]) {
    string = [string stringByAppendingString: @"\n"];
  }
  
  NSDate *today = [NSDate date];
  NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
  [dateFormat setDateFormat:@"HH:mm:ss"];
  NSString *dateString = [dateFormat stringFromDate:today];
  
  NSString *newString = [NSString stringWithFormat:@"%@: %@",dateString,string];
  
  return newString;
}

-(IBAction) didChangeWearableSwitch
{
  NSLog(@"wearableSwitch is now %d",self.scanWearableSwitch.on);
  
  if(self.scanWearableSwitch.on == YES) {
    [[NAPlatform sharedNAPlatform] startScanWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_WEARABLE]] ];
  } else {
    [[NAPlatform sharedNAPlatform] stopScanWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_WEARABLE]] ];
  }
}


-(IBAction)didChangeNewAerSwitch
{
  NSLog(@"newAerSwitch is now %d",self.scanNewAerSwitch.on);
  
  if(self.scanNewAerSwitch.on == YES) {
    [[NAPlatform sharedNAPlatform] startScanWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_NEWAER]] ];
  } else {
    [[NAPlatform sharedNAPlatform] stopScanWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_NEWAER]] ];
  }
}

-(IBAction)didChangeBeaconSwitch
{
  NSLog(@"beaconSwitch is now %d",self.scanBeaconSwitch.on);
  
  if(self.scanBeaconSwitch.on == YES) {
    [[NAPlatform sharedNAPlatform] startScanWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_BEACON]] ];
  } else {
    [[NAPlatform sharedNAPlatform] stopScanWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_BEACON]] ];
  }
}


-(IBAction)didChangeAdvertiseNewAerSwitch
{
  NSLog(@"advertiseNewAerSwitch is now %d",self.advertiseNewAerSwitch.on);
  
  if(self.advertiseNewAerSwitch.on == YES) {
    [[NAPlatform sharedNAPlatform] startAdvertisingWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_NEWAER]] name:nil ];
  } else {
    [[NAPlatform sharedNAPlatform] stopAdvertisingWithDeviceTypes:@[[NSNumber numberWithInt:NATYPE_NEWAER]] ];
  }
}

-(IBAction)didChangeLogSegment
{
  NSLog(@"log window segment changed to: %ld",(long)self.logSegment.selectedSegmentIndex);
  if(self.logSegment.selectedSegmentIndex == 0) {
    [self.self.textView setHidden:NO];
    [self.devicesTableView setHidden:YES];
    [self.sendButton setHidden:YES];
  } else {
    [self.self.textView setHidden:YES];
    [self.devicesTableView setHidden:NO];
    [self.sendButton setHidden:NO];
  }
}

-(void)updateTimerInterval:(NSTimer *)timer
{
  [self.devicesTableView reloadData];
  [self.devicesTableView selectRowAtIndexPath:self.selectedIndexPath animated:NO scrollPosition:UITableViewScrollPositionNone];
}


#pragma mark - UITableView delegates
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
  return [self.devices count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  static NSString *simpleTableIdentifier = @"DeviceTableCustomCell";
  
  DeviceTableCustomCell *cell = [tableView dequeueReusableCellWithIdentifier:simpleTableIdentifier];
  
  if (cell == nil) {
//    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:simpleTableIdentifier];
    
    NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:@"DeviceTableCustomCell" owner:self options:nil];
    cell = [topLevelObjects objectAtIndex:0];
  }
  

  if([self.devices count] == 0) {
    cell.deviceName.text = @"";
    cell.deviceId.text = @"";
    cell.deviceUpdate.text = @"";
    cell.deviceHealth.text = @"";
    cell.deviceCategory.text = @"";
    cell.appId.text = @"";
  } else {
    @synchronized(self.devices) {
      NSArray* keys = [self.devices allKeys];
      if(indexPath.row < [keys count]) {
        NADevice *device = [self.devices objectForKey:[keys objectAtIndex:indexPath.row]];
        
        cell.deviceName.text = [NSString stringWithFormat:@"[%@] %@",device.deviceTypeAsChars,device.name];
        cell.deviceId.text = [NSString stringWithFormat:@"NAUID: %@",device.deviceId];
        if(device.deviceType == NATYPE_NEWAER) {
          cell.appId.text = [NSString stringWithFormat:@"APPID: %@",device.applicationId];
        } else {
          cell.appId.text = [NSString stringWithFormat:@"SERVICE: %@",device.applicationId];
        }
        cell.deviceCategory.text = [NSString stringWithFormat:@"%@",device.deviceTypeAsString];
        cell.deviceHealth.text = [NSString stringWithFormat:@"%d",device.signalStrength];
        cell.deviceUpdate.text = [NSString stringWithFormat:@"%.2f",[[NSDate date] timeIntervalSinceDate:device.lastSeen]];
      }
    }
  }
  
  return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
  self.selectedIndexPath = indexPath;
}

- (IBAction)didClickSendMessage:(id)sender
{
  @synchronized(self.devices) {
    NSArray* keys = [self.devices allKeys];

    if(self.selectedIndexPath == nil) {
      UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                      message:@"Must select a device"
                                                     delegate:nil
                                            cancelButtonTitle:@"OK"
                                            otherButtonTitles:nil];
      [alert show];
      return;
    }
    NADevice *device = [self.devices objectForKey:[keys objectAtIndex:self.selectedIndexPath.row]];
    if(device.deviceType != NATYPE_NEWAER) {
      UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                      message:@"Cannot send to this device"
                                                     delegate:nil
                                            cancelButtonTitle:@"OK"
                                            otherButtonTitles:nil];
      [alert show];
      return;
    }
    self.devicePendingMessage = device;
    
    NSString *messageString = [NSString stringWithFormat:@"Hello %@", device.name];
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Message" message:@"Messasge to send to peer" delegate:self cancelButtonTitle:@"Cancel"  otherButtonTitles:@"Send",nil];
    alertView.alertViewStyle = UIAlertViewStylePlainTextInput;
    UITextField *messageField = [alertView textFieldAtIndex:0];
    messageField.text = messageString;
    [alertView show];
    return;
    
  }

}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
  if (buttonIndex == 0) { // Cancel
  } else if(buttonIndex == 1) { // Send
    if(self.devicePendingMessage == nil) return;
    
    NAMessage *message = [[NAMessage alloc] init];
    UITextField *messageField = [alertView textFieldAtIndex:0];
    message.data = [messageField.text dataUsingEncoding:NSUTF8StringEncoding];
    
    [[NAPlatform sharedNAPlatform] releaseDevice:self.devicePendingMessage.deviceId];
    [[NAPlatform sharedNAPlatform] sendMessage:message to:self.devicePendingMessage.deviceId];
    self.textView.text = [self.textView.text stringByAppendingString:
                     [self formatLogMsgWithString:[NSString stringWithFormat:@"M %@->%@",self.devicePendingMessage.name,messageField.text]]];
    [self.textView scrollRangeToVisible:NSMakeRange(self.textView.text.length - 1, 1)];
  }
}

@end
