//
//  NAMessage.h
//  newaer-sdk-proximity
//
//  Copyright (c) 2014 NewAer. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 NAMessage is the object that encapsulates messages for delivery to peers in the NewAer Proximity 
 Platform.
 
 */

@interface NAMessage : NSObject

typedef uint32_t NAMessageId;
typedef enum
{
  NAMESSAGE_SUCCESS = 0,    // Message was sent successfully
  NAMESSAGE_TIMEOUT = 1,    // Message was cancelled due to timeout
  NAMESSAGE_ERROR = 2       // Mesasge was not sent due to an error
} NAMessageStatus;

/** Gets or sets the NSData value associated with this message */
@property (strong) NSData *data;

-(id)init;

-(uint32_t)getMessageDataLength;
-(NSData *)getMessageData;

@end
