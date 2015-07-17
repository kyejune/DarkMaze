//
//  SoundEffectMng.cpp
//  darkmaze
//
//  Created by user on 7/6/15.
//
//

#include "SoundEffectMng.h"


bool SoundEffectMng::init(){
    if( Node::init() ){
        CCLOG("SoundEffectMng.init() %d", this->len );
        aae = AAEWrapper::create();
        aae->retain();
        return true;
    }else{
        return false;
    }
}


void SoundEffectMng::setEffectsInfomation( TMXObjectGroup* sound, DebugMap* map, Size stepSize ){
    dmap = map;
    
    len = int( sound->getObjects().size() );
   // sounds.clear();
   // pos.clear();
    
    for( auto obj : sound->getObjects())
    {
        auto xx = int( obj.asValueMap()["x"].asFloat()/stepSize.width );
        auto yy = int( obj.asValueMap()["y"].asFloat()/stepSize.height );
        
        auto ssrc = obj.asValueMap()["source"].asString();
        std::vector<std::string> ss = split( ssrc, "." );
        
        std::string path = "./";
        path.append(ss[0].c_str() );
        this->pos.push_back( Vec2( xx, yy ) );
        
        
        
        int idx = aae->addSound( path, ss[1] );
        
        //
        SoundBlock* sb = SoundBlock::create();
        sb->setLabel( ssrc );
        sb->setPosition( obj.asValueMap()["x"].asFloat(), obj.asValueMap()["y"].asFloat() );
        blocks.push_back( sb );
        
        dmap->addChild( sb );
    }
  
    
}

// 위치에 따른 vol조절
void SoundEffectMng::updateEffectsSetting( Vec2 userPosition, int userAngle ){
//    int ang;
    int dis;
    
    
    
    CCLOG("updateEffectsSetting by position %dx%d   %lu len:%d", int( userPos.x ), int( userPos.y ), this->pos.size(), this->len );
    
    userPos.set( userPosition );
    
    for( int i=0; i<this->len; i++ ){
//        SoundEffect* ae = sounds[i];
//        ang = userPos.getAngle( pos[i] );
        dis = dmap->getFastDistance( int(userPos.x), int(userPos.y), int(pos[i].x), int(pos[i].y) );
        
        float vol = 1.0f;
        if( dis > 0 ) vol = 1.0f/dis;

        
        
        
//        dis->
        blocks[i]->setDistanceLabel( std::to_string( dis ) );
        CCLOG("(%d)의 거리는 %d 이므로, 볼륨을 %f로 설정", i, dis, vol );
        aae->setVol( i, vol );
//        CCLOG("(%d)와 유저와의 거리는 %d", i, dis );
    }
    
    updateEffectsSetting( userAngle );
}

// 각도에 따른 pan조절
void SoundEffectMng::updateEffectsSetting( int userAng ){
    
        CCLOG("updateEffectsSetting by angle %d   %lu  len:%d", userAng, this->pos.size(), this->len );
    
    
    float ang;
    float pan = 0;
    for( int i=0; i<this->len; i++ ){
        
        
        ang =  atan2( pos[i].y - userPos.y, pos[i].x - userPos.x ) * 180 / 3.14159265 + userAng;
        
        if( ang < -180 ) ang += 360;
        else if( ang >= 360 ) ang -= 360;
        
        
        if( userPos.equals(pos[i]) || ang == 90 || ang == -90 ) pan = 0;
        else if( ang > -90 && ang < 90 ) pan = 1;
        else pan = -1;
        
        
        blocks[i]->setPanLabel( std::to_string( int(pan) ) );
        CCLOG( "(%d)의 유저와의 각도%dx%d ~ %dx%d 는 %f => pan:%f", i, int(userPos.x), int(userPos.y), int(pos[i].x), int(pos[i].y), ang, pan );
        aae->setPan( i, pan );
    }
}



std::vector<std::string> SoundEffectMng::split(std::string& text, std::string separators )
{
    size_t n = text.length();
    size_t start, stop;
    std::vector<std::string> result;
    start = text.find_first_not_of(separators);
    while ((start >= 0) && (start < n)) {
        stop = text.find_first_of(separators, start);
        if ((stop < 0) || (stop > n)) stop = n;
        result.push_back(text.substr(start, stop - start));
        start = text.find_first_not_of(separators, stop+1);
    }
    return result;
}