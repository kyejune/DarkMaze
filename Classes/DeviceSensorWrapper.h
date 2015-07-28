//
//  DeviceSensorWrapper.h
//  darkm
//
//  Created by user on 7/23/15.
//
//

#include "cocos2d.h"
USING_NS_CC;

class DeviceSensorWrapper: public Node
{
public:
    DeviceSensorWrapper(){};
    ~DeviceSensorWrapper(){};
    CREATE_FUNC(DeviceSensorWrapper);
    virtual bool init();
    
    void startGyro();
    
private:
    
    
};