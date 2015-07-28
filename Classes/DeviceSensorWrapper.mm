//
//  DeviceSensorWrapper.cpp
//  darkm
//
//  Created by user on 7/23/15.
//
//

#include "DeviceSensorWrapper.h"

#import "cocos2d.h"
#include <CoreMotion/CoreMotion.h>
//#include "<CoreMotion/CoreMotion.h>"

// 선언 -------------------------------------------------------------------------------------------------
@interface DeviceSensor : UIViewController
{
    CMMotionManager *motionManager;
    NSOperationQueue    *gyroQueue;
    bool isUpdating;
}

//@property (nonatomic, retain) CMMotionManager *motionManager;

@end



// 구현 -------------------------------------------------------------------------------------------------
@implementation DeviceSensor

static DeviceSensor* instance = nil;

// 싱글턴
+(DeviceSensor*)GET
{
    
    if( instance == nil )
    {
        instance = [[DeviceSensor alloc]init];
        [instance initialize];
    }
    return instance;
}

-(void)initialize
{
    motionManager = [[CMMotionManager alloc] init ];
    gyroQueue = [[NSOperationQueue alloc] init];
    if( motionManager.isGyroAvailable ) motionManager.gyroUpdateInterval = 1/60.0;
    if( motionManager.isDeviceMotionAvailable ) motionManager.deviceMotionUpdateInterval = 1/30.0;
}

-(void)toggleGyroUpdates:(bool)on
{
    if( on ) NSLog(@"toggleGyroUpdates : true" );
    else     NSLog(@"toggleGyroUpdates : false" );
    
    if( on ) {
        [ motionManager startGyroUpdatesToQueue: gyroQueue
                                    withHandler: ^(CMGyroData *gyroData, NSError *error ) {
//                                        NSLog(@"Gyro at: %f", gyroData.rotationRate );
//                                        NSLog(@"gyroUpdating");
                                        
                                    }];
    
    } else {
        
        [ motionManager stopGyroUpdates ];
        
    }
    
    isUpdating = on;
}


-(void)toggleMotionUpdates
{
    [motionManager startDeviceMotionUpdatesToQueue: gyroQueue
                                       withHandler: ^(CMDeviceMotion *motion, NSError *error) {
                                           double   yaw    = motion.attitude.yaw;
                                           double pitch    = motion.attitude.pitch;
                                           double roll     = motion.attitude.roll;
                                           
                                           
                                           
                                           NSLog(@"-----: yaw:%f, pitch:%f, roll:%f", yaw, pitch, roll );
                                       }];
}



@end



#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )


bool DeviceSensorWrapper::init(){
    [DeviceSensor GET];
    return true;
}


void DeviceSensorWrapper::startGyro(){
    [[DeviceSensor GET] toggleGyroUpdates:true];
    [[DeviceSensor GET] toggleMotionUpdates];
}


#endif
