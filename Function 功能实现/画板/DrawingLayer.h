/**
*@file 
*@brief 
*
* 由于是使用Draw 函数来画，因此每次都会将以前的画面清除掉，
* so，必须保存之前的画点位置。
*@author Created by mzy1992 on 14-2-17.
*@version 1.0
*@data 2014-10-10 16:26
*/
#ifndef __DrawingBoard__DrawingLayer__
#define __DrawingBoard__DrawingLayer__

#include <iostream>
#include "cocos2d.h"

class DrawingLayer : public cocos2d::CCLayer
{

public:
	static DrawingLayer* create();  //创建对象
	DrawingLayer();                  //构造函数
	~DrawingLayer();                 //析构函数
	bool init();                     //初始化函数

	void draw();                     //重写draw函数 绘制图像

	/// 触屏事件
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	//存放点坐标的数组
	std::vector<cocos2d::CCPoint>m_touchPoint;

	//记录每次点击的最后一点 用于识别下一次触屏的第一点 
	cocos2d::CCPoint touchEndPos;

private:
	cocos2d::CCSprite * pen; //铅笔头精灵
};
#endif /* defined(__DrawingBoard__DrawingLayer__) */
