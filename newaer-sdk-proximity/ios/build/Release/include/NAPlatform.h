//
//  NAPlatform.h
//  newaer-sdk-proximity
//
//  Copyright (c) 2014 NewAer. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NADevice.h"
#import "NAMessage.h"

@class NAPlatform;

#define kNewAerSDKVersion @"4.0"
#pragma mark - NAPlatformDelegate Protocol

///---------------------------------------------------------------------------------------
/// @name NAPlatformDelegate Protocol
///---------------------------------------------------------------------------------------

/**
 NAPlatformDelegate is the delegate protocol for the NewAer Proximity Platform.  All delegate calls are dispatched asynchronously on a private queue that may not be on the main thread.  Take care when updating UI elements based on delegate calls.
 */
@protocol NAPlatformDelegate
@required


typedef enum
{
  NAPLATFORM_DEVELOPER_KEY_INVALID = 0,         // Developer key is invalid for SDK version and bundle ID
  NAPLATFORM_INTERNET_UNAVAILABLE = 1,          // Internet connectivity has been unavailable and developer key check cannot complete
  NAPLATFORM_SDK_VERSION_UPGRADE_REQUIRED = 2,  // This SDK verison is obsolete and must be updated
  NAPLATFORM_FRIEND_KEY_INVALID = 3,            // One of the friend application IDs that was specified is invalid or not authorized
} NARegistrationFailureType;


/**
 Called when an event associated with application registration occurs.
 
 This delegate may be called any time after the platform is initialized if there is an error.  Due to network connectivity, it may not be immediate.
 
 The types of failures that can occur during application registration are:
 
     typedef enum
     {
       NAPLATFORM_DEVELOPER_KEY_INVALID = 0,         // Developer key is invalid for SDK version and bundle ID
       NAPLATFORM_INTERNET_UNAVAILABLE = 1,          // Internet connectivity has been unavailable and developer key check cannot complete
       NAPLATFORM_SDK_VERSION_UPGRADE_REQUIRED = 2,  // This SDK version is obsolete and must be updated
       NAPLATFORM_FRIEND_KEY_INVALID = 3,            // One of the friend application IDs that was specified is invalid or not authorized
     } NARegistrationFailureType;
 
 @param error The error that caused registration to fail.
 */
- (void)registrationFailed:(NARegistrationFailureType)error;

@optional

/** Called when NAPlatform discovers a new device.
 @param device The NADevice reference that was found
*/
- (void)platformDidFindDevice:(NADevice *)device;

/** Called when NAPlatform loses contact with a previously discovered device.
 @param device The NADevice reference that was lost
*/
- (void)platformDidLoseDevice:(NADevice *)device;

/** Called when NAPlatform updates the status of a previously discovered device.  This could include updates to device health, signal strength, name or when it was last seen.
 @param device The NADevice reference whose health was changed
 */
- (void)platformDidUpdateDevice:(NADevice *)device;

/** Called when NAPlatform receives a message from another device.  You will only be able to receive a message from another device if you have enabled advertising.  Not all device types can receive messages.
 @param message The message that was received by this NADevice
 @param device Which device sent the message
 */
- (void)platformDidReceiveMessage:(NAMessage *)message from:(NADevice *)device;


/** Called when NAPlatform is finished sending a message to another device.
 @param device The device that the message was sent to
 @param status The status of the message sent to the other device: success, fail, other.
 */
- (void)platformDidSendMessageToDevice:(NADevice *)device withStatus:(NAMessageStatus)status;

/** Called when NAPlatform needs user information to send to a remote peer.  If this delegate is not implemented, nothing will be returned to the peer in the user info area.  User info is restricted to 255 bytes of data.
 @param device The device that is requesting info
 @returns The NSData blob that holds the desired user info.
 */
- (NSData *)platformDidRequestDeviceInfoFrom:(NADevice *)device;

/*j
 Called when a scan cannot be started
 
 This delegate may be called any time after the scan is started, such as when the user turns off a radio or puts their device in airplane mode.
 
 @param type The scan type that scanning failed for.
 @param status Why the scan failed.
 */
- (void)platformScanFailed:(NADeviceType)type status:(NADeviceTypeStatus)status;

/** 
 Called when a scan has successfully started for a scan type.  This is called once for each device type in response to a startScanWithDeviceTypes call.
 
 @param type The scan type that scanning started for.
 */
- (void)platformDidStartScan:(NADeviceType)type;

/** 
 Called when a scan has successfully stopped for a device type.  This is called once for each device type in response to a stopScanWithDeviceTypes call.
 
 @param type The device type that scanning stopped for.
 */
- (void)platformDidStopScan:(NADeviceType)type;

/** 
 Called when one scan cycle has completed for a device type.  Some device types do not return results immediately.  This delegate is called after all didFindDevice delegates have been called.
 
 @param type The device type that scanning stopped for.
 */
- (void)platformDidCompleteOneScan:(NADeviceType)type;

/**
 Called when advertising cannot be started

 This delegate may be called any time after the advertising has started, such as when the user turns off a radio or puts their device in airplane mode.
 
 The types of failures that can occur during advertising are enumerated in NADeviceTypeStatus.
 
 @param type The device type that advertising failed for.
 */
- (void)platformAdvertiseFailed:(NADeviceType)type;

/** 
 Called when advertising has successfully started for a device type.  This is called once for each device type in response to a startAdvertisingWithDeviceTypes call.
 
 @param type The device type that advertising started for.
 */
- (void)platformDidStartAdvertise:(NADeviceType)type;

/** 
 Called when advertising has successfully stopped for a device type.  This is called once for each device type in response to a stopAdvertisingWithDeviceTypes call.
 
 @param type The device type that advertising stopped for.
 */
- (void)platformDidStopAdvertise:(NADeviceType)type;

/**
 Called when the platform has completed initialization.  Waiting for this delegate to fire ensures that all initialization is complete.  You can safely call scan and advertise functions after this delegate fires.
 */

- (void)platformInitComplete;

- (void)platformDidChangeSupportedType:(NADeviceType)type status:(NADeviceTypeStatus)status;

@end


///---------------------------------------------------------------------------------------
/// @name NAPlatform Interface
///---------------------------------------------------------------------------------------

#pragma mark - NAPlatform Interface

/**
 NAPlatform is the main class for the NewAer Proximity Platform.  It provides methods for starting and stopping scanning, and starting and stopping advertising.  NAPlatform is intended to be used as a singleton.  You must first initialized the platform before you can access the singleton.  Use [NAPlatform initializeWithApplicationId:delegate:] and then [NAPlatform sharedNAPlatform].
 
 Applications can register a delegate to receive notifications of when a device is found or lost, or if its health changes.
 */
@interface NAPlatform : NSObject {}

#pragma mark - Properties

/** Assign a delegate to receive NAPlatformDelegate messages.
 */
@property (assign) NSObject<NAPlatformDelegate> *delegate;
 
#pragma mark - Methods


typedef enum
{
  NALOGLEVEL_DEBUG = 0,
  NALOGLEVEL_INFO = 1,
  NALOGLEVEL_ERROR = 2
} NALogLevel;

/**
 Set logging level for the SDK and provide a callback function to receive log messages NALOGLEVEL_DEBUG is only available in debug builds of the SDK.
 
    typedef enum {
      NALOGLEVEL_DEBUG = 0,
      NALOGLEVEL_INFO = 1,
      NALOGLEVEL_ERROR = 2
    } NALogLevel;

 @param level Indicate what level of activity to log.  Messages lower than this level will not be output.
 @param classes An NSArray of NSString objects with the class names to output log messages for.  Use nil to display all classes.
 @param logger A block that will be called each time a log message is available
 */

+ (void)setLogLevel:(NALogLevel)level classes:(NSArray *)classes logger:(void (^)(NALogLevel level, NSString *message))logger;

/**
 Initialize NAPlatform with your developer application ID and delegate.  You must initialize the platform before you can access [NAPlatform sharedNAPlatform].
 
 You will not be able to access functions of NAPlatform until you have set the application ID.  The application ID is not checked immediately so the response reflects the most recent ID that was set.
 
 To obtain an application ID, visit newaer.com and login to the developer portal.  You will need your application's bundle identifier (ex: "com.newaer.newaer-sdk-example") to create an application ID.
 
 @param applicationId A string containing your NewAer application ID
 @param delegate The delegate that implements the NAPlatformDelegate protocol
 */
+ (BOOL)initializeWithApplicationId:(NSString *)applicationId delegate:(NSObject<NAPlatformDelegate> *)delegate;

/**
 NAPlatform is a singleton and this is the only way to access the instance.
 */
+ (NAPlatform *)sharedNAPlatform;

/**
 Register additional application IDs that this application is compatible with.  Some features of the SDK use the application ID to scan for other devices and services.  Register additional application IDs before you begin scanning to ensure that you will detect devices advertising with those IDs.  If you only need to scan for devices matching your registered application ID, you don't need to call this function.

 @param applicationIds An NSArray containing applicationIds to register, For example: @[ @"ac3bd3f7-7d1d-4132-94f2-f9ef0594eeb5",@"b97eb066-8f89-497d-b6f4-a1d11e48a383", ...]
 */
- (BOOL)registerFriendApplicationIds:(NSArray *)applicationIds;

/**
 Register iBeacon IDs that this application is compatible with. Register additional iBeacon IDs before you begin scanning to ensure that you will detect devices with those IDs.  If you only need to scan for devices matching your registered application ID, you don't need to call this function.
 
 @param beaconIds An NSArray containing beaconIds to register, For example: @[ @"ac3bd3f7-7d1d-4132-94f2-f9ef0594eeb5", ...]
*/
 
- (BOOL)registerBeaconIds:(NSArray *)beaconIds;



///---------------------------------------------------------------------------------------
/// @name Scanning
///---------------------------------------------------------------------------------------

/**
 Starts scanning for a specific scan family.
 
 The [NAPlatformDelegate platformScanFailed:status:] delegate will be called if there was a problem starting scanning for any device type.
 
 The [NAPlatformDelegate platformDidStartScan:] delegate will be called if the scan was started successfully.
 
 The [NAPlatformDelegate platformDidCompleteOneScan:] delegate will be called each time a round of scanning completes.  Use this message to determine when to stop scanning or when to send results to another module for processing.
 
 Once the scan has started, the [NAPlatformDelegate platformDidFindDevice:] and [NAPlatformDelegate platformDidLoseDevice:] delegates will be called each time a device is found or lost.
 
 (iOS Only) Scanning is only intended to be used while the application is in the foreground.  If the application is in the background or the device is locked, certain types of scans may be disabled or have degraded performance.
 
 @param deviceTypes Which device types to enable scanning for.  Pass an NSArray of NSNumber objects.  Each NSNumber object should take the value of one of the NADeviceType eunmerated values.
*/

- (void)startScanWithDeviceTypes:(NSArray*)deviceTypes;

/**
 Stops scanning for specific device types.
 
 The [NAPlatformDelegate platformDidStopScan:] delegate will be called when the scan was stopped successfully.
 
 @param deviceTypes Which device types to disable scanning for. Pass an NSArray of NSNumber objects.  Each NSNumber object should take the value of one of the NADeviceType eunmerated values.  Pass nil to stop scanning for all device types.
 */
- (void)stopScanWithDeviceTypes:(NSArray *)deviceTypes;


///---------------------------------------------------------------------------------------
/// @name Advertising
///---------------------------------------------------------------------------------------

/**
 Start advertising for the specified device types.  The only scan type that currently supports advertising is NASCAN_NEWAER.
 
 The [NAPlatformDelegate platformAdvertiseFailed:] delegate will be called if there was a problem starting advertising for any device type.
 
 The [NAPlatformDelegate platformDidStartAdvertise:] delegate will be called if the advertising was started successfully.

 @param deviceTypes Which device types to enable advertising for.   Pass an NSArray of NSNumber objects.  Each NSNumber object should take the value of one of the NADeviceType eunmerated values.  Pass nil to start advertising on all available device types.
 @param name A name that will be used when advertising.  If nil, the current operating system provided device name is used.
 */

- (void)startAdvertisingWithDeviceTypes:(NSArray *)deviceTypes name:(NSString *)name;

/**
 Stops advertising for specific device types.
 The [NAPlatformDelegate platformDidStopAdvertise:] delegate will be called if the advertise was stopped successfully.

 @param deviceTypes Which scan types to stop advertising for.   Pass an NSArray of NSNumber objects.  Each NSNumber object should take the value of one of the NADeviceType eunmerated values.  Pass nil for all device types.
 */
- (void)stopAdvertisingWithDeviceTypes:(NSArray *)deviceTypes;


///---------------------------------------------------------------------------------------
/// @name Messaging
///---------------------------------------------------------------------------------------

/**
 Queues a message to be sent to a device ID.  The device ID need not be visible currently and may come into range in the future.  The message send request does not timeout and only one message may be queued at a time.  Calling this method a second time overwrites the first message.  Only peer devices of scan type NASCAN_NEWAER can receive messages. When you stop scanning, all queued messages are discarded.
 
 See the [NAPlatformDelegate platformDidSendMessageToDevice:withStatus:] delegate to be informed of message status.

 @param message The message to send to the device.
 @param deviceId Which device ID to send the message to.  This value is typically obtained from an NADevice object.
 */
-(void)sendMessage:(NAMessage *)message to:(NSString *)deviceId;

/**
 Releases a device identified by deviceId from the scanning cache.  Use this to get new peer info on the next scan.
 @param deviceId Which device ID to send the message to.  This value is typically obtained from an NADevice object.
 */
- (void)releaseDevice:(NSString *)deviceId;

///----
/// @name Platform information
///----

/**
 Gets the deviceId for this device for the corresponding NADeviceType
 
 @returns A deviceId string that can be used to uniquely identify your application.
 */
-(NSString *)getMyDeviceId;

@end

