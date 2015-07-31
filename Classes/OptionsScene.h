#ifndef __OPTIONSCENE_SCENE_H__
#define __OPTIONSCENE_SCENE_H__

//
//  OptionsScene.h
//  darkm
//
//  Created by user on 7/30/15.
//
//

#include "cocos2d.h"

class OptionsScene : public cocos2d::Scene
{
    
private:
    static OptionsScene *instance;
    OptionsScene(){};
    ~OptionsScene(){};
    
public:
    static OptionsScene *getInstance();
    static void releaseInstance();
    virtual bool init();
    
};

#endif // __OPTIONSCENE_SCENE_H__