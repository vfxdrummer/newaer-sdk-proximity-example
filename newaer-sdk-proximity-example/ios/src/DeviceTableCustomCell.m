//
//  DeviceTableCustomCell.m
//  newaer-sdk-proximity-example
//
//  Copyright (c) 2013 NewAer. All rights reserved.
//

#import "DeviceTableCustomCell.h"

@implementation DeviceTableCustomCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
  self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
  if (self) {
      // Initialization code
  }
  return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
  [super setSelected:selected animated:animated];

  // Configure the view for the selected state
}

@end
