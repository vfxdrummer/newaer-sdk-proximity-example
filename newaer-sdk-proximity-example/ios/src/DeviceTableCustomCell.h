//
//  DeviceTableCustomCell.h
//  newaer-sdk-proximity-example
//
//  Copyright (c) 2013 NewAer. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DeviceTableCustomCell : UITableViewCell

@property (assign) IBOutlet UILabel *deviceName;
@property (assign) IBOutlet UILabel *deviceCategory;
@property (assign) IBOutlet UILabel *deviceHealth;
@property (assign) IBOutlet UILabel *deviceUpdate;
@property (assign) IBOutlet UILabel *deviceId;
@property (assign) IBOutlet UILabel *appId;

@end
