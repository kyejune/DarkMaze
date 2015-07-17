//
//  FPAlertView.cpp
//  FBLeaderboard
//
//  Created by degineer on 6/30/14.
//
//



#include "AAEWrapper.h"

//obj c 타입
#import "AEAudioController.h"
#import "AEAudioFilePlayer.h"
#import "AEAudioUnitFilter.h"


// 선언 ----------------------------------------------------------------------------------------------------------------------------------------
@interface AEAudioControllerWrapper : NSObject
{
//    AEAudioUnitFilter* reverbF;
}
@property (nonatomic, strong ) AEAudioController *audioController;
@property (nonatomic, strong ) AEAudioUnitFilter *reverbFilter;


+(AEAudioControllerWrapper*)GET;
-(void)initialize;

@end

// 구현 ----------------------------------------------------------------------------------------------------------------------------------------
@implementation AEAudioControllerWrapper

static AEAudioControllerWrapper* instance = nil;

//대충 만든 싱글턴..
+(AEAudioControllerWrapper*)GET
{
    
    if( instance == nil )
    {
        instance = [[AEAudioControllerWrapper alloc]init];
        [instance initialize];
    }
    return instance;
}

+(AEAudioController*)CONTROLLER
{
    
    return [[AEAudioControllerWrapper GET] audioController];
}

-(AEAudioController*) audioController
{
    return _audioController;
}

-(AEAudioUnitFilter *)reverbFilter{
    return _reverbFilter;
}


-(void)initialize
{

    self.audioController = [[AEAudioController alloc]
                            initWithAudioDescription:[AEAudioController interleaved16BitStereoAudioDescription]
                            inputEnabled:NO];
    
    NSError *error = NULL;
    BOOL result = [_audioController start:&error];
    
    // 필터 생성
    AudioComponentDescription component = AEAudioComponentDescriptionMake(kAudioUnitManufacturer_Apple,
                                                                          kAudioUnitType_Effect,
                                                                          kAudioUnitSubType_Reverb2);
    
    _reverbFilter = [[AEAudioUnitFilter alloc]
               initWithComponentDescription:component
               audioController:_audioController
               error:&error];
    if ( !_reverbFilter ) {
        // Report error
    }
    
    AudioUnitSetParameter( _reverbFilter.audioUnit, kReverb2Param_DryWetMix, kAudioUnitScope_Global, 0, 80.f, 0);
    
    
    //
    if( !result ){
        NSLog(@"audioController isn't started" );
    }else{
        NSLog(@"사운드 초기화 성공");
    }
}

//- (void)didReceiveMemoryWarning {
//    [super didReceiveMemoryWarning];
//    // Dispose of any resources that can be recreated.
//}

@end




#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )



//- (void)ready{
//    CCLOG("READY");
//}


// c++ 타입 해터에 대응하는 구현부  ----------------------------------------------------------------------------------------------------------------------------------------
//  -----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AAEWrapper::init(){
    [AEAudioControllerWrapper GET];
    return true;
}

void AAEWrapper::reset(){
    CCLOG("사운드 리셋 및 정지");
}

int AAEWrapper::addSound( std::string filePath, std::string extention ){
//    CCLOG("사운드 추가: %s", filePath.c_str() );
    
    
    NSString *fileString = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filePath.c_str()]
                                                  ofType:[NSString stringWithUTF8String:extention.c_str()]];
    NSURL *file = [NSURL fileURLWithPath:fileString ];
    AEAudioFilePlayer *a = [AEAudioFilePlayer audioFilePlayerWithURL:file
                            audioController:[AEAudioControllerWrapper CONTROLLER] error:NULL ];

    [[AEAudioControllerWrapper CONTROLLER] addChannels:@[a]];
    
    return int( [[[AEAudioControllerWrapper CONTROLLER] channels ] count ] );
}

void AAEWrapper::setVol( int idx, float vol ){
//    CCLOG("유저 위치에 맞춰서 조절" );
    [[[AEAudioControllerWrapper CONTROLLER] channels][idx] setVolume:vol];
}

void AAEWrapper::setPan( int idx, float pan ){
    [[[AEAudioControllerWrapper CONTROLLER] channels][idx] setPan:pan];
}

void AAEWrapper::reverbTo( int idx, bool on ){
    CCLOG("%d번 사운드의 reverb를 %s", idx, on?"켬":"끔" );
    if( on )
    {
        [[[AEAudioControllerWrapper CONTROLLER] channels][idx]
            addFilter:[[AEAudioControllerWrapper GET] reverbFilter ]];
    }
    else{
        [[[AEAudioControllerWrapper CONTROLLER] channels][idx]
            removeFilter:[[AEAudioControllerWrapper GET] reverbFilter ]];
    }
}




#endif

