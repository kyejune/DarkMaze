//
//  SoundBlock.cpp
//  TilemapTest
//
//  Created by user on 4/10/15.
//
//

#include "SoundBlock.h"



bool SoundBlock::init()
{
    
    if( DrawNode::init() )
    {
        
//        this->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
//        
        
        label = Label::createWithSystemFont("S", "Arial", 6 );
        label->setColor( Color3B::RED );
        label->setAlignment( TextHAlignment::CENTER );
        label->setPosition( 7, 4 );
        
        
        
        panLabel      = Label::createWithSystemFont("P:", "Arial", 6 );
        distanceLabel = Label::createWithSystemFont("D:", "Arial", 6 );
        
        distanceLabel->setPosition( 5, -4 );
        panLabel->setPosition( 5, -12 );
        
        
        
        this->addChild( label );
        this->addChild( panLabel );
        this->addChild( distanceLabel );
        
        
        return true;
    };
    
    
    return  false;
}


void SoundBlock::setLabel( std::string str ){
    label->setString( str );
}

void SoundBlock::setPanLabel( std::string str ){
    panLabel->setString( std::string("P:").append( str ) );
}

void SoundBlock::setDistanceLabel( std::string str ){
    distanceLabel->setString( std::string("D:").append( str ) );
}