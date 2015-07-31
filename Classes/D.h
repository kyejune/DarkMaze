//
//  Common.h
//  TilemapTest
//
//  Created by user on 4/22/15.
//
//

#ifndef __D__
#define __D__

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
        LEFT,
        HOLD
    };
    
    enum Load
    {
        PASS,
        BLOCKED,
        CLOSED_DOOR,
        OUTLINE
    };
}


//class D{

    
   
//public:
//    
//    static float mapWidth;
//    static float mapHeight;

//};



#endif // __D__
