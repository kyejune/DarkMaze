//
//  DebugMap.cpp
//  TilemapTest
//
//  Created by user on 6/25/15.
//
//

#include "DebugMap.h"

USING_NS_CC;


bool DebugMap::init()
{

    if( DrawNode::init() ){
        return true;
    }else{
        return false;
    }
}

void DebugMap::parseMap( cocos2d::TMXTiledMap* tmap  ){
    
    TMXObjectGroup* wall = tmap->getObjectGroup("linewall");
    TMXObjectGroup* door = tmap->getObjectGroup("door");
    
    Size mapSize  = tmap->getContentSize();
    tilesLen = tmap->getMapSize();
    tilesSize= tmap->getTileSize()/2;
    
    
    CCLOG("parseMap %f %f", D::mapSize.width, D::mapSize.height );
    
    for( int i=0; i<tilesLen.height+1; i++ ) this->drawLine( Vec2( 0, i*(tilesSize.height)), Vec2( mapSize.width, i*(tilesSize.height) ), Color4F::BLUE );
    for( int j=0; j<tilesLen.width+1; j++ )  this->drawLine( Vec2( j*(tilesSize.width), 0 ), Vec2( j*(tilesSize.width), mapSize.height ), Color4F::BLUE );

    
//    CCLOG(" Line... %f, %f", tmap->getMapSize().width, tmap->getMapSize().height ); // 갯수
//    CCLOG(" Line... %f, %f", tmap->getTileSize().width, tmap->getTileSize().height ); // 크기
    
    DrawNode* wallNode = DrawNode::create();
    this->addChild( wallNode );
    
    DrawNode* doorNode = DrawNode::create();
    this->addChild( doorNode );
    
    int lineCount = 0;
    for( auto obj : wall->getObjects())
    {
        bool first = true;
        Vec2 prevPoint;
        
        
        auto xx = obj.asValueMap()["x"].asFloat();
        auto yy = obj.asValueMap()["y"].asFloat();
        
        // 벽 생성 및 위치
        for( auto value : obj.asValueMap() )
        {
            if( value.first == "polylinePoints"){
                wallSeg.push_back( *new std::vector<Vec2> );
                auto vec = value.second.asValueVector();
                
//                CCLOG("catch Line %d", lineCount );
                first = true;
                for( auto &p : vec ){
                    Vec2 point = Vec2( ( p.asValueMap().at("x").asFloat()*.5 ) + xx, -( p.asValueMap().at("y").asFloat()*.5 ) + yy );
                    
                    if( first == false ){
                        wallNode->drawSegment( prevPoint, point, 3.0f, Color4F::WHITE );
                    }
                    
                    wallSeg[lineCount].push_back(point);
                    prevPoint = point;
                    first = false;
                }//for
                
//                CCLOG( "(%d) line length is %lu", lineCount, wallSeg[lineCount].size());
                lineCount += 1;
            }//if
            
        }//for
    }
    
    int doorLineCount = 0;
    // 문 그리기
    for( auto obj: door->getObjects())
    {
        bool first = true;
        Vec2 prevPoint;
        
        auto xx = obj.asValueMap()["x"].asFloat();
        auto yy = obj.asValueMap()["y"].asFloat();
        
        // 벽 생성 및 위치
        for( auto value : obj.asValueMap() )
        {
            if( value.first == "polylinePoints"){
                doorSeg.push_back( *new std::vector<Vec2> );
                auto vec = value.second.asValueVector();
                
                first = true;
                for( auto &p : vec ){
                    Vec2 point = Vec2( ( p.asValueMap().at("x").asFloat()*.5 ) + xx, -( p.asValueMap().at("y").asFloat()*.5 ) + yy );
                    
                    if( first == false ){
                        doorNode->drawSegment( prevPoint, point, 3.0f, Color4F::RED );
                    }
                    
                    doorSeg[doorLineCount].push_back(point);
                    prevPoint = point;
                    first = false;
                }//for
                
                //                CCLOG( "(%d) line length is %lu", lineCount, wallSeg[lineCount].size());
                doorLineCount += 1;
            }//if
            
        }//for
    }
}

D::Load DebugMap::checkLoad( Vec2 begin, Vec2 target ){
//    CCLOG( "checkLoad %dx%d --> %dx%d", int(start.x), int(start.y), int(target.x), int(target.y));
    if( target.x < 0 || target.y < 0 || target.x > tilesLen.width || target.y > tilesLen.height ) return D::Load::BLOCKED;
    
    Vec2 loadBegin = Vec2( begin.x*tilesSize.width + tilesSize.width/2, begin.y*tilesSize.height + tilesSize.height/2 );
    Vec2 loadEnd   = Vec2( target.x*tilesSize.width + tilesSize.width/2, target.y*tilesSize.height + tilesSize.height/2 );
    
    
    long wallLen = wallSeg.size();
    for( int i=0; i<wallLen; i++ ){
        for( int j=0; j<wallSeg[i].size()-1; j++ ){
            Vec2 SegS = wallSeg[i][j];
            Vec2 SegE = wallSeg[i][j+1];
            if( Vec2::isSegmentIntersect( loadBegin, loadEnd, SegS, SegE ) ) return D::Load::BLOCKED;
        }
    }
    
    return D::Load::PASS;
}

bool DebugMap::checkHasClosedDoor( std::vector<Vec2> route ){
    for( int i=1; i<route.size(); i++ ){
        if( checkDoor( route[i-1], route[i]) == D::Load::CLOSED_DOOR ) return true;
    }
    
    return false;
}

D::Load DebugMap::checkDoor( Vec2 begin, Vec2 target ){
    if( target.x < 0 || target.y < 0 || target.x > tilesLen.width || target.y > tilesLen.height ) return D::Load::OUTLINE;
    
    Vec2 loadBegin = Vec2( begin.x*tilesSize.width + tilesSize.width/2, begin.y*tilesSize.height + tilesSize.height/2 );
    Vec2 loadEnd   = Vec2( target.x*tilesSize.width + tilesSize.width/2, target.y*tilesSize.height + tilesSize.height/2 );
    
    
    long doorLen = doorSeg.size();
    for( int i=0; i<doorLen; i++ ){
        for( int j=0; j<doorSeg[i].size()-1; j++ ){
            Vec2 SegS = doorSeg[i][j];
            Vec2 SegE = doorSeg[i][j+1];
            if( Vec2::isSegmentIntersect( loadBegin, loadEnd, SegS, SegE ) ) return D::Load::CLOSED_DOOR;
        }
    }
    
    return D::Load::PASS;
}



int DebugMap::getFastDistance( int startCoordX, int startCoordY, int endCoordX, int endCoordY ){
    CCLOG("<<<<<<<<<<<< getFastDistance %dx%d ~ %dx%d", startCoordX, startCoordY, endCoordX, endCoordY );
    std::vector<Vec2> route;
    start.setPoint( startCoordX, startCoordY );
    goal.setPoint( endCoordX, endCoordY );
    
    hasLastRouteClosedDoor = false;
    
    // 클리어
    closedCoords.clear();
    if ( start.equals(goal) ){
        route.push_back( start );
        route.push_back( goal );
        getLastSoundSourcePos( route );
        return 0;
    }
    
    
    // 탐색 시작
    // 맨처음 출발지가 여러개라면 모두 출발해보고 제일 짧은걸로 선택
    int minDistance = INT_MAX;
    std::vector<Vec2> startingPoints = getOpenTiles( start );
    
    for( int i=0; i<startingPoints.size(); i++ ){
        route.clear();
        route.push_back( start );
        
        // 한칸짜리 이동
        Vec2 nextStart = startingPoints[i];
        if( nextStart.equals( goal )){
            route.push_back( goal );
            getLastSoundSourcePos( route );
            hasLastRouteClosedDoor = ( checkDoor(nextStart, goal) == D::Load::CLOSED_DOOR );
            return 1;
        }
        
        closedCoords.push_back( start );
        parentMap[coordToIndex(nextStart)] = coordToIndex( start );
        
        
        // 한칸 이상이면..
        if( findNext( nextStart, &route ) ){
            int dis = accumulateDistance( coordToIndex(goal), 0, nextStart );
            if( dis < minDistance ){
                minDistance = dis;
                
                route.push_back( goal );
                getLastSoundSourcePos( route );
                hasLastRouteClosedDoor = checkHasClosedDoor( route );
            }
        }
        
        closedCoords.clear();
        parentMap.clear();
    }

    
    if( minDistance == INT_MAX ) return -1;
    else                         return minDistance;
    
}


int DebugMap::accumulateDistance( int mapIndex, int accumulatedDistance, Vec2 startingPoint ){
    if( mapIndex == coordToIndex( startingPoint )) return accumulatedDistance + 1;
    else                                           return accumulateDistance( parentMap[mapIndex], accumulatedDistance +1, startingPoint );
}

// route에 저장된 경로를 토대로 각 사운드 시작점, 코너, 도착점에 대한 좌표를 추출, getFastDistance를 호출할때마다 새로 계산됨
// 이것도 저장해두고 같은걸 물어볼때는 꺼내오면 될듯??
void DebugMap::getLastSoundSourcePos( std::vector<Vec2> route ){
    std::vector<Vec2> r;
    
    long len = route.size();
    if( len >= 3 ){ // 최저한으로 코너가 생길수 있는 길이
        for( int i=1; i<len-1; i++ ){
            Vec2 prev = route[i-1];
            Vec2 next = route[i+1];
            
            float ang = atan2( prev.y - next.y, prev.x - next.x ) * 180 / 3.14159265;
            if( std::abs(ang) != 90 && std::abs(ang) != 180 && ang != 0 ) r.push_back( route[i] );
        }
    }
    
    
    lastSoundPos = r;
}

std::vector<Vec2> DebugMap::getOpenTiles( Vec2 coord ){
    std::vector<Vec2> result;
    
    // 중심을 기준으로 4방향중 이동가능한 타일 좌표를 추출
    Vec2 t = Vec2( coord.x, coord.y+1 );
    Vec2 r = Vec2( coord.x+1, coord.y );
    Vec2 b = Vec2( coord.x, coord.y-1 );
    Vec2 l = Vec2( coord.x-1, coord.y );
    
    if( !isMarkedCoord( t ) &&  checkLoad( coord, t ) == D::PASS ){
       result.push_back( t );
    }
    if( !isMarkedCoord( r ) &&  checkLoad( coord, r ) == D::PASS ){
       result.push_back( r );
    }
    if( !isMarkedCoord( b ) &&  checkLoad( coord, b ) == D::PASS ){
        result.push_back( b );
    }
    if( !isMarkedCoord( l ) &&  checkLoad( coord, l ) == D::PASS ){
        result.push_back( l );
    }
    
    // 직선거리가 가까워보이는 순으로 정렬
    std::sort(result.begin(), result.end(), CC_CALLBACK_2( DebugMap::compareDistance, this ));
    
//    CCLOG("그래서 진입가능한 루트는 총 %d곳", int( result.size()) );
    
    return result;
}

bool DebugMap::compareDistance( Vec2 a, Vec2 b ){ // true면 a가 앞으로, false면 b가 앞으로
    
    bool ar = (checkLoad( a, goal ) == D::PASS);
    bool br = (checkLoad( b, goal ) == D::PASS);
    
    if( ar && !br ) return true;
    else if( !ar && br ) return false;
    
    return a.distance(goal) < b.distance(goal);
}

//
bool DebugMap::findNext( Vec2 coord, std::vector<Vec2>* route ){
    closedCoords.push_back( coord );
    route->push_back( coord );
    // 진행가능한 좌표 추출
    std::vector<Vec2> nextEntries = getOpenTiles( coord );
    
    if( nextEntries.size() == 0 ){
        return false;
    }
    
    
    for( int i=0; i<nextEntries.size(); i++ ){
        Vec2 n = nextEntries[i];
        // 기록..
        parentMap[coordToIndex(n)] = coordToIndex( coord );

        
        // 목표에 도착하면 종료
        if( nextEntries[i].equals( goal ) == true ){
            return true;
        }
        else // 아니면 계속 검색
        {
            if( findNext( n, route ) ) return true;
        }
    }
    
    return false;
}



int DebugMap::coordToIndex( Vec2 coord ){
    return coord.x + coord.y*tilesLen.width;
}


// 외길일때는 문제 없는데 아니면 course 번호별로 검색하는 루틴 추가
bool DebugMap::isMarkedCoord( Vec2 coord ){
    for( int i=0; i<closedCoords.size(); i++ ){
        if( closedCoords[i].equals( coord ) ){
            return true;
        }
    }
    return false;
}
