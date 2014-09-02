//
//  NADevice.h
//  newaer-sdk-proximity
//
//  Copyright (c) 2014 NewAer. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum
{
  NATYPE_NEWAER = 1,       // NewAer SDK device
  NATYPE_BEACON = 2,       // Beacon device
  NATYPE_WEARABLE = 3,     // Wearable device
  NATYPE_RESERVED = 4      // Reserved
} NADeviceType;


typedef enum
{
  NAPLATFORM_DEVICE_TYPE_AVAILABLE = 0,             // Scanning or advertising for this device type is possible on the hardware, not known whether it is enabled
  NAPLATFORM_DEVICE_TYPE_ENABLED = 1,               // Available and enabled
  NAPLATFORM_DEVICE_TYPE_NOT_AVAILABLE = 2,         // Scanning or advertising for this device type is not possible on the hardware
  NAPLATFORM_DEVICE_TYPE_ERROR = 3                  // Scanning or advertising for this device type has failed
} NADeviceTypeStatus;

/**
 The representation of a device in the system.  A device can be used from the time that NAPlatform
 informs your delegate of its presence until the time it is notified to be lost.
 
 Devices have an NADeviceType associated with them:
 
     typedef enum
     {
       NATYPE_NEWAER = 1,       // NewAer SDK device
       NATYPE_BEACON = 2,       // Beacon device
       NATYPE_WEARABLE = 3,     // Other wearable device
       NATYPE_RESERVED = 4      // Reserved
     } NADeviceType;

 */

@interface NADevice : NSObject

/** Returns the device type as a string: "NewAer", "Beacon", "Wearable" */
- (NSString *)deviceTypeAsString;
+ (NSString *)deviceTypeAsString:(NADeviceType)type;

/** Returns the device type as a character string prefix: "NA", "IB", "WE" */
- (NSString *)deviceTypeAsChars;
+ (NSString *)deviceTypeAsChars:(NADeviceType)deviceType;

/** Returns the full name of the device: "John Smith's iPhone" */
@property (copy) NSString *name;

/** Returns the device type as an integer: TYPE_NEWAER, TYPE_BEACON, etc */
@property (assign) NADeviceType deviceType;

/** Returns the device ID, a unique name not intended for display to the end user */
@property (copy) NSString *deviceId;

/** Returns the application ID that the device is associated with.  Only valid for NASCAN_NEWAER devices.  applicationId may be helpful to determine the device's purpose if you have registered for friend applicationIds. */
@property (copy) NSString *applicationId;

/** Returns the contents of the remote device info characteristic */
@property (copy) NSData *deviceInfo;

/** Returns the date that the device was last seen */
@property (copy) NSDate *lastSeen;

/** Returns the raw signal strength for the device. */
@property (assign) int32_t signalStrength;

/** Returns the signal level scaled to 0-100 */
@property (assign) int32_t signalLevel;

- (NSString *)description;
@end

