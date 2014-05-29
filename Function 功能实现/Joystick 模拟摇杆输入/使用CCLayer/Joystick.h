#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__	

#include "cocos2d.h"  

using namespace cocos2d;  

class Joystick :public CCLayer 
{  
public :  
	void Active();  
	void Inactive();  
	CCPoint getDirection();  
	float getVelocity();  
	void  updatePos(float ft);  

	//初始化 aPoint是摇杆中心 aRadius是摇杆半径 aJsSprite是摇杆控制点 aJsBg是摇杆背景  
	static Joystick*  JoystickWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg);  
	Joystick * initWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg);  

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);  
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);  
private:
	CCPoint centerPoint;//摇杆中心  
	CCPoint currentPoint;//摇杆当前位置  
	bool active;//是否激活摇杆  
	float radius;//摇杆半径  
	CCSprite *jsSprite;  
	CREATE_FUNC(Joystick);  
};  
#endif