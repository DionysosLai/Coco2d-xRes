/**
*@file HelpPrompt.h
*@brief 新手提示
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-11-4 15:17
*/

#ifndef __HELP_PROMPT_H__
#define __HELP_PROMPT_H__

#include "cocos2d.h"

enum 
{
	HELP_PROMPT_E_RECTANGLE,
	HELP_PROMPT_E_CIRCLE,
};

class HelpPrompt : public cocos2d::CCLayer 
{
public:
	HelpPrompt();
	virtual ~HelpPrompt();
///@brief 创建一个长方形提示框，默认整个蒙板为全屏，提示框为长方形为100*100
///@return 
	static HelpPrompt* Create(const cocos2d::CCPoint& pos);
	virtual bool init(const cocos2d::CCPoint& pos);

	virtual void onEnter();
	virtual void onExit();

//	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
public:
///@brief 提示框类型
///@param[in]  HELP_PROMPT_RECTANGLE--长方形，HELP_PROMPT_CIRCLE--圆形
///@return 
	CC_PROPERTY(unsigned short, m_sPromptType, PromptType);
///@brief 宽度设置与获取
///@param[in] m_fWidth---宽度
///@pre 提示框方式是长方形
///@return 
	CC_PROPERTY(float, m_fWidth, Width);
///@brief 高度设置与获取
///@param[out] m_fHeight---高度
///@pre 提示框方式是长方形
///@return 
	CC_PROPERTY(float, m_fHeight, Height);
///@brief 半径设置与获取
///@param[in] m_fRadius---半径
///@pre 提示框方式是圆形
///@return 
	CC_PROPERTY(float, m_fRadius, Radius);
///@brief 位置设置与获取
///@param[in] m_ptLocation---位置 
///@return 
	CC_PROPERTY(cocos2d::CCPoint, m_ptLocation, Location);
///@brief 是否传递触摸消息
///@param[in] m_bSwallow--是否传递触摸消息
///@return true--传递， false--不传递
	CC_PROPERTY(bool, m_bSwallow, Swallow);
private:
	void initData(const cocos2d::CCPoint& pos);
///@brief 是否要分发触摸消息
///@param[in] pos --- 坐标位置 
///@return true---分发 false--不分发
	bool bSwallowTouch(const cocos2d::CCPoint& pos);
};
#endif	///< (__HELP_PROMPT_H__)