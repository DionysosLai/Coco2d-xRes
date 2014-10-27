#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "S2CSprite.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject* pSender);
    
    CREATE_FUNC(HelloWorld);

	virtual void draw();

	void drawLighting(float x1, float y1, float x2, float y2, float displace);

private: 
	S2CSprite* m_sp;

public:
	virtual void onEnter();

};

#endif // __HELLOWORLD_SCENE_H__
