#include "Joystick.h" 

USING_NS_CC;

void Joystick::updatePos(float ft)
{  
	jsSprite->setPosition(ccpAdd(jsSprite->getPosition(),ccpMult(ccpSub(currentPoint, jsSprite->getPosition()),0.5)));  
}  

void Joystick::Active()  
{  

	if (!active) 
	{  
		active = true;  
		schedule(schedule_selector(Joystick::updatePos));//添加刷新函数  
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);//添加触摸委托  
	}
	else 
	{  

	}  
}  
//冻结摇杆  
void Joystick::Inactive()  
{  
	if (active) 
	{  
		active=false;  
		this->unschedule(schedule_selector(Joystick::updatePos));	///< 删除刷新  
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->removeDelegate(this);		///< 删除委托  
	}
	else 
	{  

	}  
}  

bool Joystick::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)  
{  
	if (!active)  
	{
		return false;  
	}

	CCPoint touchPoint = pTouch->getLocationInView();
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint);  

	if (ccpDistance(touchPoint, centerPoint) > radius) 
	{
		return false;  
	}
	currentPoint = touchPoint;  

	return true;  
}  

void Joystick::ccTouchMoved(CCTouch *pTouch, CCEvent *pEven)  
{  
	CCPoint touchPoint = pTouch->getLocationInView();  
	touchPoint = CCDirector:: sharedDirector()->convertToGL(touchPoint);  

	if (ccpDistance(touchPoint, centerPoint) > radius)  
	{  
		currentPoint =ccpAdd(centerPoint,ccpMult(ccpNormalize(ccpSub(touchPoint, centerPoint)), radius));  
	}
	else 
	{  
		currentPoint = touchPoint;  
	}  
}  

void Joystick::ccTouchEnded(CCTouch *pTouch, CCEvent *pEven)  
{  
	currentPoint = centerPoint;  
}  

//获取摇杆方位,注意是单位向量  
CCPoint Joystick::getDirection()  
{  
	return ccpNormalize(ccpSub(centerPoint, currentPoint));  
}  

//获取摇杆力度  
float Joystick::getVelocity()  
{  
	return ccpDistance(centerPoint, currentPoint);  
}  

Joystick* Joystick:: JoystickWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg)
{  
	Joystick * jstick=Joystick::create();  
	jstick->initWithCenter(aPoint,aRadius,aJsSprite,aJsBg);  
	return jstick;  
}  

Joystick* Joystick::initWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg)
{  
	active = false;  
	radius = aRadius;  
	centerPoint = aPoint;  
	currentPoint = centerPoint;  
	jsSprite = aJsSprite;  
	jsSprite->setPosition(centerPoint);  
	aJsBg->setPosition(centerPoint);  

	this->addChild(aJsBg);  
	this->addChild(jsSprite);  	
	return this;  
}  
