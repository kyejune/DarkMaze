//
//  DebugMap.h
//  TilemapTest
//
//  Created by user on 6/25/15.
//
//

#ifndef __TilemapTest__DebugMap__
#define __TilemapTest__DebugMap__

#include <stdio.h>
#include <unordered_map>
#include "D.h"


USING_NS_CC;

class DebugMap: public cocos2d::DrawNode{
  
public:
    DebugMap(){};
    ~DebugMap(){};
    CREATE_FUNC(DebugMap);
    virtual bool init();
    
    
    void parseMap( cocos2d::TMXTiledMap* tmap );
    D::Load checkLoad( Vec2 start, Vec2 target );
    D::Load checkDoor( Vec2 start, Vec2 target );
    bool checkHasClosedDoor( std::vector<Vec2> route );
    
    int getFastDistance( int startCoordX, int startCoordY, int endCoordX, int endCoordY );
    
    Size tilesLen;
    Size tilesSize;
    
    std::vector<Vec2> lastSoundPos;
    bool hasLastRouteClosedDoor;
    
//    Vec2 posToCoord( Vec2 position );
//    Vec2 posToCoord( int x, int y );
    
    
private:
    float PTM_RATIO = 32;
    Vec2 goal;// = Vec2(0,0);
    Vec2 start;// = Vec2(0,0);
    std::vector< std::vector<Vec2>> wallSeg;
    std::vector< std::vector<Vec2>> doorSeg;
    std::unordered_map< int, int > parentMap = {};
    std::vector<Vec2> closedCoords;
    
    std::vector<Vec2> getOpenTiles( Vec2 coord );
    bool findNext( Vec2 coord, std::vector<Vec2>* route );
    bool isMarkedCoord( Vec2 coord );
    bool compareDistance( Vec2 a, Vec2 b );
    
//    std::vector<Vec2> lastSoundPos;
    void getLastSoundSourcePos( std::vector<Vec2> route );
    
    int accumulateDistance( int mapIndex, int accumulatedDistance,  Vec2 startingPoint );
    int coordToIndex( Vec2 coord );
};


#endif /* defined(__TilemapTest__DebugMap__) */
