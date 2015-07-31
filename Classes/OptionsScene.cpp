//
//  OptionsScene.cpp
//  darkm
//
//  Created by user on 7/30/15.
//
//

#include "OptionsScene.h"

USING_NS_CC;


// ~~~~~~~~~~~~~~~~~~~~ Singletone Instance
OptionsScene *OptionsScene::instance = NULL;
OptionsScene *OptionsScene::getInstance()
{
    if( instance == NULL ){
        instance = new OptionsScene();
        instance->init();
    }
    
    return instance;
}

void OptionsScene::releaseInstance()
{
    if( instance ) delete instance;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// on "init" you need to initialize your instance
bool OptionsScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    return true;
}
