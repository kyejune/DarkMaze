//
//  Common.h
//  TilemapTest
//
//  Created by user on 4/22/15.
//
//

#ifndef __TilemapTest__D__
#define __TilemapTest__D__

#include <stdio.h>

USING_NS_CC;

namespace D {
//    static float mapWidth;
//    static float mapHeight;
//    static cocos2d::
    
    static Size mapSize;
    static Size canvasScale;
    
//    static float canvasScaleX;
//    static float canvasScaleY;
    
//    static float stepW;
//    static float stepH;
    static Size stepSize;
//    static Vec2 stepMax;
    
//    static float stepWidthMax = 12;
//    static float stepHeightMax = 16;
    
    
    enum Direction
    {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT
    };
    
    enum Load
    {
        PASS,
        BLOCKED,
        CLOSED_DOOR
    };
}


//class D{

    
   
//public:
//    
//    static float mapWidth;
//    static float mapHeight;

//};



#endif /* defined(__TilemapTest__D__) */
