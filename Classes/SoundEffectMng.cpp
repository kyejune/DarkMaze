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
        
        
        aae->addSound( path, ss[1] );
        
        hasReverb.push_back( 0 );
        
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
    
    sndSrcPos.clear();
    userPos.set( userPosition );
    
    for( int i=0; i<this->len; i++ ){
        dis = dmap->getFastDistance( int(userPos.x), int(userPos.y), int(pos[i].x), int(pos[i].y) );
        
        float vol = 1.0f;
        if( dis > 0 ) vol = 1.0f/dis;

        blocks[i]->setDistanceLabel( std::to_string( dis ) );
        aae->setVol( i, vol );
        
//        CCLOG("(%d)의 거리는 %d 이므로, 볼륨을 %f로 설정", i, dis, vol );
//        this->runAction( ActionTween::create( 0.8f, "vol", aae->getVol(i), vol ) );
//        CCLOG("(%d)와 유저와의 거리는 %d", i, dis );
        
//        std::vector<Vec2> ssp = dmap->getLastSoundSourcePos(<#std::vector<Vec2> route#>)
        
        std::vector<Vec2> v;
        sndSrcPos.push_back( v );
        sndSrcPos[i] = dmap->lastSoundPos;
        
        bool prevHas = (hasReverb[i]!=0);
        bool has = (dmap->hasLastRouteClosedDoor);
        
        if( has != prevHas ) aae->reverbTo( i, has );
        hasReverb[i] = has;
    }
    
    updateEffectsSetting( userAngle );
}

// 각도에 따른 pan조절
void SoundEffectMng::updateEffectsSetting( int userAng ){
    
//        CCLOG("updateEffectsSetting by angle %d   %lu  len:%d", userAng, this->pos.size(), this->len );
    float ang;
    float pan = 0;
    for( int i=0; i<this->len; i++ ){
        
        
        
        std::vector<Vec2> ssp = sndSrcPos[i];
        for( int j=0; j<ssp.size(); j++ ){
            CCLOG("(%d)번 사운드사이의 갈림길은 %dx%d", i, int(ssp[j].x), int(ssp[j].y) );
        }
        

        if( ssp.size() > 0 ) ang = atan2( ssp[0].y - userPos.y, ssp[0].x - userPos.x ) * 180 / 3.14159265 + userAng;
        else                 ang = atan2( pos[i].y - userPos.y, pos[i].x - userPos.x ) * 180 / 3.14159265 + userAng;
        
        if( ang < -180 ) ang += 360;
        else if( ang >= 360 ) ang -= 360;
        
        
        if( userPos.equals(pos[i]) || ang == 90 || ang == -90 ) pan = 0;
        else if( ang > -90 && ang < 90 ) pan = 1;
        else pan = -1;
        
        
        blocks[i]->setPanLabel( std::to_string( int(pan) ) );
        
        std::string pstr = "pan:";
        pstr.append( std::to_string(i) );
        
        this->runAction( ActionTween::create( 0.8f, pstr, aae->getPan(i), pan ) );
    }
}




void SoundEffectMng::updateTweenAction(float value, const std::string &key)
{
    auto str = std::string( key );
    std::vector<std::string> keyValue = split( str, ":" );
    auto keystr = keyValue[0];
    auto keyIdx = std::stoi( keyValue[1] );
    
    if( keystr == "pan") aae->setPan( keyIdx, value );
}


std::vector<std::string> SoundEffectMng::split(std::string &text, std::string separators )
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