//
//  StageScene.cpp
//  darkm
//
//  Created by user on 7/30/15.
//
//

#include "StageScene.h"

// ~~~~~~~~~~~~~~~~~~~~ Singletone Instance
StageScene *StageScene::instance = NULL;
StageScene *StageScene::getInstance()
{
    if( instance = NULL ){
        instance = new StageScene();
        instance->init();
    }
    
    return instance;
}

void StageScene::releaseInstance()
{
    if( instance ) delete instance;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// on "init" you need to initialize your instance
bool StageScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    return true;
}
