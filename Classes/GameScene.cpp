#include "GameScene.h"

USING_NS_CC; //== using namespace cocos2d;




// ~~~~~~~~~~~~~~~~~~~~ Singletone Instance
GameScene *GameScene::instance = NULL;
GameScene *GameScene::getInstance()
{
    if( instance == NULL ){
        instance = new GameScene();
        instance->init();
    }
    
    return instance;
}

void GameScene::releaseInstance()
{
    if( instance ) delete instance;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// on "init" you need to initialize your instance
bool GameScene::init()
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
    swipeListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    swipeListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    swipeListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    swipeListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority( swipeListener, this );
    
    
    // 기울기 관련
    Device::setAccelerometerEnabled( true );
    auto accListener = EventListenerAcceleration::create( CC_CALLBACK_2( GameScene::OnAcceleration, this ));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this );
    
//    this->addChild( gameLayer );
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
    
//    isMoving = false;
    
    
    // 마스크 테스트
    sightStencil = DrawNode::create();
    sightStencil->drawSolidCircle(Vec2(0,0), 100, 0, 50, 1, 1, Color4F::WHITE );
    
    
    ClippingNode *cn = ClippingNode::create( sightStencil );
    cn->addChild( gameLayer );
    cn->setInverted( false );
    this->addChild( cn );
    sightStencil->setPosition( visibleSize*.5 );
    
    
    return true;
}


// --------------------- AccelerationEvent

void GameScene::OnAcceleration( cocos2d::Acceleration *acc, cocos2d::Event *event){
    
    float accX = round(acc->x*100);
    float accY = round(acc->y*100);
    
    float tiltX = std::abs( averageX - accX );
    float tiltY = std::abs( averageY - accY );
    
//    bool movable = false;
    
    // tilting가능할때..
    if( ud->getNumberOfRunningActions() == 0 ){
        
        if( prevAccXXX.size() >= 100 ){
         
            // 평균값이랑 비교해서 너무 튀면 알려주기
            if( tiltX > tiltY && tiltX > 60 )
            {
                if( accX > 0 ) setLineOfSight( D::LEFT );
                else           setLineOfSight( D::RIGHT);
            }
            else if( tiltY > tiltX && tiltY > 30 )
            {
                holdingTiltY   = tiltY;
                maxOffsetTiltY = 0;
                
                if( accY > 0 ) setCoordByDirection(D::TOP);
                else           setCoordByDirection(D::BOTTOM);
            }else{
                
                
                if( maxOffsetTiltY > 20 ){
                
                prevAccXXX.erase( prevAccXXX.begin() );
                prevAccYYY.erase( prevAccYYY.begin() );
                
                
                prevAccXXX.push_back( accX );
                prevAccYYY.push_back( accY );
                
                averageX = std::accumulate(prevAccXXX.begin(), prevAccXXX.end(), 0.0)/prevAccXXX.size();
                averageY = std::accumulate(prevAccYYY.begin(), prevAccYYY.end(), 0.0)/prevAccYYY.size();
                
                }
                
            }
            
        }else{
            
            
            prevAccXXX.push_back( accX );
            prevAccYYY.push_back( accY );
            
            averageX = std::accumulate(prevAccXXX.begin(), prevAccXXX.end(), 0.0)/prevAccXXX.size();
            averageY = std::accumulate(prevAccYYY.begin(), prevAccYYY.end(), 0.0)/prevAccYYY.size();
        }
    }
    // user이동중인상태: 폰기울기가 고정되었는지를 확인해서 고정이면 반복액션, 움직임이 있었으면 초기상태로..
    else
    {
        if( prevAccYYY.size() >= 200 ){
            maxOffsetTiltY = std::max( maxOffsetTiltY, holdingTiltY - tiltY );
//            CCLOG("홀딩중에 기울기 offsetY: %f", maxOffsetTiltY );
        }
    }
}

// --------------------- TouchEvent

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    isTouchDown = true;
    
    // Get the touch point
    Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
    isTouchDown = false;
}

void GameScene::onTouchCancelled(Touch *touch, Event *event)
{
    onTouchEnded(touch, event);
}

void GameScene::update(float dt)
{
    if (true == isTouchDown)
    {
        int offsetX = std::abs( initialTouchPos[0] - currentTouchPos[0] );
        int offsetY = std::abs( initialTouchPos[1] - currentTouchPos[1] );
        
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
    
    fixUserAtCenter();
}


void GameScene::setLineOfSight( D::Direction direction ){
    
    CCLOG("setLineOfSight: %lu", ud->getNumberOfRunningActions());
//    if( ud->isRunning()  ) CCLOG("setLineOfSight 기다려" );
//    else                   CCLOG("setLineOfSight 진행");
    
    
    if( ud->getNumberOfRunningActions() > 0 ) return;
    
    lastOrder = direction;
    
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
    
    ud->stopAllActions();
    gameLayer->runAction(RotateTo::create( 0.5, nextRot ) );
//    ud->runAction( Sequence::create( RotateTo::create( 0.5, -nextRot ),
//                                     CallFunc::create( CC_CALLBACK_0( GameScene::doneMoving, this )), NULL ));
    
    ud->runAction(RotateTo::create( 0.5, -nextRot ));
    
    
    sMng->updateEffectsSetting( -nextRot );
//    isMoving = true;
}

void GameScene::setCoord( Vec2 targetCoord, bool nonAnimation, bool ignoreRule ){
    
    CCLOG("setCoord: %lu", ud->getNumberOfRunningActions());
    
    if( ud->getNumberOfRunningActions() > 0 ) return;
    
//    if( ud->isRunning()  ) CCLOG("setCoord 기다려" );
//    else                   CCLOG("setCoord 진행");
    
//    if( isMoving ) return;
//    isMoving = true;
    
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
    
    ud->stopAllActions();
    if( nonAnimation )
    {
        ud->setPosition( nextPos );
    }
    else{
        ud->runAction( MoveTo::create( 0.5, nextPos ));
//        ud->runAction( Sequence::create( MoveTo::create( 0.8, nextPos ),
//                                        CallFunc::create( CC_CALLBACK_0( GameScene::doneMoving, this )), NULL ));
        
    }
    
    sMng->updateEffectsSetting( targetCoord, ud->getRotation() );
}


void GameScene::fixUserAtCenter(){
    
    Size size = Director::getInstance()->getWinSize();
    Vec2 center = Vec2( size.width/2, size.height/2 );
    Vec2 offset = rotatePoint( Vec2(0,0), ud->getPosition(), gameLayer->getRotation() );
    
    gameLayer->setPosition( center-offset );
}

//void GameScene::doneMoving(){
////    isMoving = false;
//}


Vec2 GameScene::rotatePoint( Vec2 anchor, Vec2 point, float angle ){
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


void GameScene::setCoordByDirection( D::Direction direction ){
    
    const int d[4] = { 0, 90, 180, -90 };
    int swipeR     = d[direction];
    int containerR = std::floor( gameLayer->getRotation() + 0.5 );
    
    lastOrder = direction;
    
    CCLOG( "%u >>>>>>> %d, - %d, = %d", direction, containerR, swipeR, containerR-swipeR );
    
    switch ( (containerR-swipeR)%360 ) {
        case 0:
            setCoord( currentCord + Vec2( 0, 1 ));
            break;
            
            
        case 180:
        case -180:
            setCoord( currentCord - Vec2( 0, 1 ));
            break;
            
            
        case 90:
        case -270:
            setCoord( currentCord - Vec2( 1, 0 ));
            break;
            
        case -90:
        case 270:
            setCoord( currentCord + Vec2( 1, 0 ));
            break;
            
        default:
            
            break;
    }
}