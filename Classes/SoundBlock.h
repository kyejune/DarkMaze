//
//  SoundBlock.h
//  TilemapTest
//
//  Created by user on 4/10/15.
//
//

#ifndef __TilemapTest__SoundBlock__
#define __TilemapTest__SoundBlock__

#include "cocos2d.h"

USING_NS_CC;

class SoundBlock: public cocos2d::DrawNode
{
    
    
    
    
public:
    SoundBlock(){};
    ~SoundBlock(){};
    
    CREATE_FUNC(SoundBlock);
    virtual bool init();
    
    
    void setPanLabel( std::string str );
    void setDistanceLabel( std::string str );
    void setLabel( std::string str );
    
    
    
private:
    Label* label;
    Label* distanceLabel;
    Label* panLabel;
    
};



#endif // __TilemapTest__SoundBlock__