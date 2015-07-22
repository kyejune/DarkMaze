#include "MainScene.h"


USING_NS_CC; //== using namespace cocos2d;

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    gameLayer = Layer::create();

    // 맵 생성 및 사이즈 맞추기
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->tmapBg = TMXTiledMap::create("./darkmaze.tmx");
    
    D::mapSize = Size( tmapBg->getContentSize().width, tmapBg->getContentSize().height );
    D::canvasScale = Size( visibleSize.width/D::mapSize.width, visibleSize.height/D::mapSize.height );
    D::stepSize = Size( tmapBg->getTileSize().width/2, tmapBg->getTileSize().height/2 );
    
    dmap = DebugMap::create();
    sMng = SoundEffectMng::create();
    sMng->retain();
    this->addChild( sMng, 0 ); // Action사용 위해서 화면에 붙임
    
    
    // 유저 생성
    ud = UserDot::create();
    
    // 출구 위치 기록
//    TMXObjectGroup* obj = tmapBg->getObjectGroup("obj");
//    ValueMap exit = obj->getObject("exit");
//    exitPos = b2Vec2( (exit["x"].asFloat() + exit["width"].asFloat()*.5)/PTM_RATIO,  (exit["y"].asFloat() - exit["height"].asFloat()*.5)/PTM_RATIO );
    //    exitX = exit["x"].asFloat() + exit["width"].asFloat()*.5;
    //    exitY = exit["y"].asFloat() - exit["height"].asFloat()*.5;
    
    
    // 맵 파싱
//    parseMap( std::string( "./D.tmx" ) );
    
    // touch 관련
    auto swipeListener = EventListenerTouchOneByOne::create();
    swipeListener->setSwallowTouches(true);
    swipeListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    swipeListener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    swipeListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    swipeListener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority( swipeListener, this );
   
    
    this->addChild( gameLayer );
    gameLayer->addChild( tmapBg, 0 );
    gameLayer->addChild( ud, 1 );
    gameLayer->setContentSize( Size( D::mapSize.width, D::mapSize.height ) );
    gameLayer->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    
    
    sMng->setEffectsInfomation( tmapBg->getObjectGroup("sound"), dmap, D::stepSize );
    dmap->parseMap( tmapBg );
    gameLayer->addChild( dmap );
    
    
    setCoord( Vec2( 8, 0 ), true, true );
    
    fixUserAtCenter();
    this->scheduleUpdate();
    
    isMoving = false;
    
    return true;
}



// --------------------- TouchEvent

bool MainScene::onTouchBegan(Touch *touch, Event *event)
{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    isTouchDown = true;
    
    // Get the touch point
    Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
    CCLOG("Touch: %f %f", touchLocation.x, touchLocation.y);
    
    
//    dmap->getColorMap();
    
    return true;
}

void MainScene::onTouchMoved(Touch *touch, Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    //    CCLOG("onTouchMoved");
}

void MainScene::onTouchEnded(Touch *touch, Event *event)
{
    isTouchDown = false;
    
    //    CCLOG("onTouchEnd");
}

void MainScene::onTouchCancelled(Touch *touch, Event *event)
{
    onTouchEnded(touch, event);
    
    //    CCLOG("onTouchCancelled");
}

void MainScene::update(float dt)
{
    
//        CCLOG("update %s", isTouchDown?"o":"x" );
    
    
    if (true == isTouchDown)
    {
        int offsetX = std::abs( initialTouchPos[0] - currentTouchPos[0] );
        int offsetY = std::abs( initialTouchPos[1] - currentTouchPos[1] );
        
        
        
        //       MathUtil.roundf( offsetX );
        //        MathUtil.abs(MathUtil.roundf(offsetY) );
        
        if(  offsetX > offsetY ){
            if (initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05)
            {
                CCLOG("SWIPED LEFT");
                
                isTouchDown = false;
                setLineOfSight( D::LEFT );
            }
            else if (initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05)
            {
                CCLOG("SWIPED RIGHT");
                
                isTouchDown = false;
                setLineOfSight( D::RIGHT );
            }
        }else{
            if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05)
            {
                CCLOG("SWIPED DOWN");
                
                isTouchDown = false;
                setCoordByDirection( D::TOP );
            }
            else if (initialTouchPos[1] - currentTouchPos[1] < - visibleSize.width * 0.05)
            {
                CCLOG("SWIPED UP");
                
                isTouchDown = false;
                setCoordByDirection( D::BOTTOM );
            }
        }
    }
    
//    if( ud->getNumberOfRunningActions() > 0 || gameLayer->getNumberOfRunningActions() >  0 ){
        fixUserAtCenter();
//    }
    
//    CCLOG( "???????????? %d", ud->numberOfRunningActions() );
}


void MainScene::setLineOfSight( D::Direction direction ){
    if( isMoving ) return;
    
    switch ( direction ) {
        case D::RIGHT:
            nextRot = gameLayer->getRotation() + 90;
            break;
            
        case D::LEFT:
            nextRot = gameLayer->getRotation() - 90;
            break;
            
        default:
            break;
    }
    
    gameLayer->runAction(RotateTo::create( 0.5, nextRot ) );
//    ud->runAction(RotateTo::create( 0.5, -nextRot ));
    ud->runAction( Sequence::create( RotateTo::create( 0.5, -nextRot ),
                                     CallFunc::create( CC_CALLBACK_0( MainScene::doneMoving, this )), NULL ));
    
    
    sMng->updateEffectsSetting( -nextRot );
    isMoving = true;
}

void MainScene::setCoord( Vec2 targetCoord, bool nonAnimation, bool ignoreRule ){
    if( isMoving ) return;
    CCLOG( "setCoord %f, %f", targetCoord.x, targetCoord.y  );
    
    if( ignoreRule == false ){
        
        switch ( dmap->checkLoad( currentCord, targetCoord ) ) {
            case D::PASS:
                CCLOG("이동");
                break;
                
            default:
                CCLOG("이동못함");
                return;
                break;
        }
    }
    
    
    Vec2 prev = ud->getPosition();
    nextPos = Vec2( targetCoord.x*D::stepSize.width + D::stepSize.width/2, targetCoord.y*D::stepSize.height + D::stepSize.height/2 );
   
    currentCord.x = targetCoord.x;
    currentCord.y = targetCoord.y;
  
    if( nonAnimation )
    {
        ud->setPosition( nextPos );
    }
    else{
        ud->runAction( Sequence::create( MoveTo::create( 0.5, nextPos ),
                                        CallFunc::create( CC_CALLBACK_0( MainScene::doneMoving, this )), NULL ));
        
    }
    
    sMng->updateEffectsSetting( targetCoord, ud->getRotation() );
    
    isMoving = true;
}


void MainScene::fixUserAtCenter(){
    
    Size size = Director::getInstance()->getWinSize();
    Vec2 center = Vec2( size.width/2, size.height/2 );
    Vec2 offset = rotatePoint( Vec2(0,0), ud->getPosition(), gameLayer->getRotation() );
    
    gameLayer->setPosition( center-offset );
}

void MainScene::doneMoving(){
    isMoving = false;
}


Vec2 MainScene::rotatePoint( Vec2 anchor, Vec2 point, float angle ){
    float rad = angle * 0.017453;
    float s = sin(rad);
    float c = cos(rad);
    
    // translate point back to origin:
    point.x -= anchor.x;
    point.y -= anchor.y;
    
    // rotate point
    float xnew = point.x * c + point.y * s;
    float ynew = -point.x * s + point.y * c;
    
    // translate point back:
    point.x = xnew + anchor.x;
    point.y = ynew + anchor.y;
    
    return point;
}


void MainScene::setCoordByDirection( D::Direction direction ){
    
    const int d[4] = { 0, 90, 180, -90 };
    int swipeR     = d[direction];
    int containerR = std::floor( gameLayer->getRotation() + 0.5 );
    
    CCLOG( "%u >>>>>>> %d, - %d, = %d", direction, containerR, swipeR, containerR-swipeR );
    
    switch ( (containerR-swipeR)%360 ) {
        case 0:
            setCoord( currentCord + Vec2( 0, 1 ));
//            setCoord( coordinates[0], coordinates[1] + 1 );
            break;
            
            
        case 180:
        case -180:
            setCoord( currentCord - Vec2( 0, 1 ));
//            setCoord( coordinates[0], coordinates[1] - 1 );
            break;
            
            
        case 90:
        case -270:
            setCoord( currentCord - Vec2( 1, 0 ));
//            setCoord( coordinates[0] - 1, coordinates[1] );
            break;
            
        case -90:
        case 270:
            setCoord( currentCord + Vec2( 1, 0 ));
//            setCoord( coordinates[0] + 1, coordinates[1] );
            break;
            
        default:
            
            break;
    }
}