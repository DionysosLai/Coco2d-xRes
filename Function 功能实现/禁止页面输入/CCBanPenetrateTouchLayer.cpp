/**
*@file CCBanPenetrateTouchLayer.cpp
*@brief 禁止页面输入的具体代码
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-5-29 15:25
*/
CCBanPenetrateTouchLayer::~CCBanPenetrateTouchLayer()
{

}

bool CCBanPenetrateTouchLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	setTouchEnabled(true);

	return true;
}

bool CCBanPenetrateTouchLayer::ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent )
{
	return true;//返回true表示吞噬触摸消息,不在往下传递
}

void CCBanPenetrateTouchLayer::ccTouchMoved( CCTouch* pTouch, CCEvent* pEvent )
{

}

void CCBanPenetrateTouchLayer::ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent )
{

}

void CCBanPenetrateTouchLayer::registerWithTouchDispatcher()
{
	//使用-128和CCMenu优先级相同,并且吞掉事件true//  
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);  
	 
	CCLayer::registerWithTouchDispatcher();  
}