//
//  UserDot.cpp
//  TilemapTest
//
//  Created by user on 4/10/15.
//
//

#include "UserDot.h"


//UserDot* UserDot::create()
//{
//    UserDot* instance = new(std::nothrow) UserDot();
//    
//    if( instance && instance->init() )
//    {
//        instance->autorelease();
//    }
//    else
//    {
//        CC_SAFE_DELETE( instance );
//    }
//    
//
//    
//    return instance;
//}


bool UserDot::init()
{
    
    
//    Director::getInstance()->getTextureCache();
    //==
//    TextureCache::getInstance();
    
//    _director->getTextureCache()
    
//    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("userdot.png");
    
//    Sprite::initWithTexture( texture );
    
//    if( this->initWithFile("userdot.png") )
    if( DrawNode::init() )
    {
//        this->initWithFile("");
        
//        float radius = this->getBoundingBox().size.width * .5;
//        float r = radius*.5;
        
        
//        DrawNode* left = DrawNode::create();
//        left->drawCircle(Vec2(-r, radius), r, 0, 4, true, 1, 1, Color4F::Color4F::BLUE );
//        this->addChild( left );
//        
//        DrawNode* right = DrawNode::create();
//        right->drawCircle(Vec2(radius*2 + r, radius), r, 0, 4, true, 1, 1, Color4F::Color4F::BLUE );
//        this->addChild( right );
        
        float w = 10;//this->getBoundingBox().size.width;
        float h = 10;//this->getBoundingBox().size.height;
        
        this->drawSolidCircle(Vec2(0,0), w/2, 0, 16, 1, 1, Color4F::RED );
        this->drawTriangle( Vec2( -w/4, 0 ), Vec2( 0, h-2 ), Vec2( w/4, 0 ), Color4F::Color4F::RED );
        
//        DrawNode* frame = DrawNode::create();
//        frame->drawRect(Vec2(0,0), Vec2(w, h ), Color4F::Color4F::ORANGE );
//        
//        
//        DrawNode* head = DrawNode::create();
//        head->drawTriangle( Vec2( w/2, h + 4 ), Vec2( 4, h ), Vec2( w-4, h ), Color4F::Color4F::GREEN );
        
        
        
//        this->addChild(frame);
//        this->addChild( head );
        
        
        return true;
    };
    
    
    return  false;
}
