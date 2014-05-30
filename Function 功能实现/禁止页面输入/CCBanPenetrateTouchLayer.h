/**
*@file CCBanPenetrateTouchLayer.h
*@brief Ω˚÷π“≥√Ê ‰»Î
*
*http://post.justbilt.com/2013/07/05/cocos2d-x-%E6%B8%B8%E6%88%8F%E5%AE%9E%E6%88%98%E7%BB%8F%E9%AA%8C%E4%B8%80-%E8%A7%A6%E6%91%B8%E5%93%8D%E5%BA%94/
*
*@author DionysosLai£¨email: 906391500@qq.com 
*@version 1.0
*@data 2014-5-29 15:22
*/
#ifndef _H_CCBANPENETRATETOUCHLAYER_H_
#define _H_CCBANPENETRATETOUCHLAYER_H_

#include "cocos2d.h"

class CCBanPenetrateTouchLayer : public cocos2d::CCLayer
{
public:
	CREATE_FUNC(CCBanPenetrateTouchLayer);

	CCBanPenetrateTouchLayer();
	~CCBanPenetrateTouchLayer();
public:
	virtual bool init();
	virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);  
	virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);  
	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);

	virtual void registerWithTouchDispatcher();
};

#endif // (_H_CCBANPENETRATETOUCHLAYER_H_)