//
//  StageScene.h
//  darkm
//
//  Created by user on 7/30/15.
//
//

#ifndef __darkm__StageScene__
#define __darkm__StageScene__

#include <stdio.h>


class StageScene : public cocos2d::Scene
{
private:
    static StageScene *instance;
    StageScene(){};
    ~StageScene(){};
    
public:
    static StageScene *getInstance();
    static void releaseInstance();
    virtual bool init();
    
};



#endif /* defined(__darkm__StageScene__) */
