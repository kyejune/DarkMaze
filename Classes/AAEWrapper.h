//
//  ViewController.h
//  coreaudiounit
//
//  Created by user on 6/2/15.
//  Copyright (c) 2015 kyejune. All rights reserved.
//

//#import <UIKit/UIKit.h>
//#import "AEAudioFilePlayer.h"
//#import "AEBlockChannel.h"
//#import "AEAudioUnitFilter.h"

#include "cocos2d.h"
USING_NS_CC;

class AAEWrapper: public Node
{
public:
    AAEWrapper(){};
    ~AAEWrapper(){};
    CREATE_FUNC(AAEWrapper);
    virtual bool init();
    
    void reset();
    int addSound( std::string filePath, std::string extention );
    
    void setPan( int idx, float pan );
    float getPan( int idx );
    
    // user위치값 입력받아서 사운드 위치랑 비교해서 gain, pan, volume조절
    void setVol( int idx, float vol );
    float getVol( int idx );
    
    // 문열림 닫힘 상태에 따라 해당 사운드 필터 on/off
    void reverbTo( int index, bool on );
    
//    CC_SYNTHESIZE( float, panValue, Pan );
    
    
private:
    

};
