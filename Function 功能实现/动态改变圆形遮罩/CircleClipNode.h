/**
*@file CircleClipNode.h
*@brief 实现圆形遮罩效果
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-5-30 10:27
*/

#ifndef __CIRCLE_CLIPNODE_H__
#define __CIRCLE_CLIPNODE_H__

#include "cocos2d.h"

enum 
{
	TAGCLIPNODE,
};

class CircleClipNode : public cocos2d::CCNode
{
public:
	CircleClipNode();
	~CircleClipNode();
public:
	///@brief 创建圆心遮罩
	///@param[in] cen--圆心 radius--半径 radiusDelta--- 每次半径渐变大小， radiusEnd--当半径大于或小于某一值时，设置运动结束 bInverted--true--显示“底板-模板内容 ”
	///@pre 前者条件
	///@return 说明
	static CircleClipNode* create(cocos2d::CCPoint cen, float radius, float radiusDelta, float radiusEnd, bool bInverted);
	virtual bool init(cocos2d::CCPoint cen, float radius, float radiusDelta, float radiusEnd, bool bInverted);

	CC_PROPERTY(bool, m_bEnd, End);
private:
	void initData(cocos2d::CCPoint cen, float radius, float radiusDelta, float radiusEnd, bool bInverted);

	virtual void update(float delta);
private:
	float m_fRadius;				///< 半径
	cocos2d::CCPoint m_pointCen;	///< 圆心
	float m_fRadiusDelta;			///< 半径
	float m_fRadiusEnd;				///< 半径大于或小于多少时，遮罩设置结束

protected:

};
#endif // (__CIRCLE_CLIPNODE_H__)
