#ifndef __INTROSCENE_SCENE_H__
#define __INTROSCENE_SCENE_H__

#include "cocos2d.h"

class IntroScene : public cocos2d::Scene
{
private:
    static IntroScene *instance;
    IntroScene(){};
    ~IntroScene(){};
    
public:
    static IntroScene *getInstance();
    static void releaseInstance();
    virtual bool init();
    
//    CREATE_FUNC(<#__TYPE__#>)
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
};




#endif // __INTROSCENE_SCENE_H__
