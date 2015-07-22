//
//  SoundEffectMng.h
//  darkmaze
//
//  Created by user on 7/6/15.
//
//

#ifndef __darkmaze__SoundEffectMng__
#define __darkmaze__SoundEffectMng__

#include <stdio.h>
#include "AAEWrapper.h"
#include "DebugMap.h"
#include "SoundBlock.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class SoundEffectMng: public cocos2d::Node, public ActionTweenDelegate
{
  
public:
    SoundEffectMng(){};
    ~SoundEffectMng(){};
    CREATE_FUNC(SoundEffectMng);
    virtual bool init();
    virtual void updateTweenAction(float value, const std::string& key);
    
    void setEffectsInfomation( TMXObjectGroup* sound, DebugMap* map, Size stepSize );
    void updateEffectsSetting( Vec2 userPos, int userAng );
    void updateEffectsSetting( int userAng );
    
    
private:
    AAEWrapper* aae;
    DebugMap* dmap;
    std::vector<SoundBlock*> blocks;
    std::vector<Vec2> pos;
    
    Vec2 userPos;
    
    int len = 0;
    std::vector<std::string> split(std::string& text, std::string separators );

};

#endif /* defined(__darkmaze__SoundEffectMng__) */
