#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "UserDot.h"
#include "D.h"
#include "DebugMap.h"
#include "SoundEffectMng.h"


class MainScene : public cocos2d::Scene
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(MainScene);
    
    void OnAcceleration( cocos2d::Acceleration *acc, cocos2d::Event *event );
//    void onClosedAlert( EventCustom* evt );
    int parseMap( std::string fileName );
    int getApproximateDegree( int degree );
    
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    bool isTouchDown;
    bool isMoving;
    
    cocos2d::Size visibleSize;
    
    float initialTouchPos[2];
    float currentTouchPos[2];
    
    void update(float dt);
    void doneMoving();

    
    Vec2 currentCord;
    void setCoord( Vec2 targetCoord, bool nonAnimation = false, bool ignoreRule = false );
    void setCoordByDirection( D::Direction direction );
    void setLineOfSight( D::Direction direction );
    
    
    
    //// OVERRIDE ////
//    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
//    

private:
    
    float MOVING_TIME = 0.4;
    
    float PTM_RATIO = 32;
    
    cocos2d::Vec2 prevPos;
    cocos2d::Vec2 curPos;
    
    float m_accelerateFactor = 100;
    float m_accelerateRate   = .1;
    Vec2 nextPos;
    float nextRot = 0;
    
    double accX;
    double accY;
    
    void getNewPosition( double ax, double ay );
    void fixUserAtCenter();
    
    

    
    TMXTiledMap* tmapBg;
    Layer* gameLayer;
    DebugMap* dmap;
    SoundEffectMng* sMng;
    
    UserDot* ud;
    
    Vec2 rotatePoint( Vec2 anchor, Vec2 point, float angle );
    
};




#endif // __HELLOWORLD_SCENE_H__
