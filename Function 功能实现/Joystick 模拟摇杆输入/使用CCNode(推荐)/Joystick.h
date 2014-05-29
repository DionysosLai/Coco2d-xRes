/**
*@file Joystick.h
*@brief Joystick 类定义
*
*
*
*@author 原作者Cocos2der http://blog.csdn.net/cocos2der/article/details/6912519
*	     更改：赖武功 Dionysos
*@version 1.0(maj.min，主版本.分版本格式)
*@data 2014-03-21
*/
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__	

#include "cocos2d.h"  

//@brief 
///
///
using namespace cocos2d;  

///@brief 简要概述
///
///注意在OnEnter中创建委托机制和OnExit中移除委托机制
///
///
///
///
///
///@invariant
///
class Joystick :public CCNode, public CCTargetedTouchDelegate
{  
public :  
	void Active();  
	void Inactive();  
	CCPoint getDirection();  
	float getVelocity();  
	void  updatePos(float ft);  

	//初始化 aPoint是摇杆中心 aRadius是摇杆半径 aJsSprite是摇杆控制点 aJsBg是摇杆背景  
	static Joystick* JoystickWithCenter(CCPoint aPoint,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg);  
	static Joystick* JoystickWithCenter(CCPoint aPoint, CCSprite* aJsSprite, CCSprite* aJsBg);
	Joystick* initWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg);  

	/// 判断点是否在中心摇杆内
	bool isPointInJoystick(const CCPoint touchPoint);

	/// 触摸操作
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);  
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);  

	/// 触摸协议
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

	virtual void onEnter(void);
	virtual void onExit(void);
private:
	CCPoint centerPoint;//摇杆中心  
	CCPoint currentPoint;//摇杆当前位置  
	bool active;//是否激活摇杆  
	float radius;//摇杆半径  
	CCSprite *jsSprite;  
	CREATE_FUNC(Joystick);  
};  
#endif