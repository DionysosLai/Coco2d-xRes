/**
*@file CircleActionBy.h
*@brief 圆周运动
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-10-9 17:34
*/

#ifndef __CIRCLE_ACTION_H__
#define __CIRCLE_ACTION_H__

#include "cocos2d.h"

class CircleActionBy : public cocos2d::CCActionInterval
{
public:
	CircleActionBy();
	~CircleActionBy();

	/// 创建动作：时间 圆心 半径
	static CircleActionBy* create(float t, cocos2d::CCPoint circleCenter, float radius);

	bool init(float t, cocos2d::CCPoint circleCenter, float radius);

    virtual void startWithTarget(cocos2d::CCNode *pTarget);

    virtual void update(float time);
private:

protected:
	cocos2d::CCPoint m_ptCircleCenter;	///< 圆心
	float m_fRadius;					///< 半径
	float m_fRadian;					///< 弧度
	float m_fMoveTimes;					///< 刷新次数
};
#endif		///< (__CIRCLE_ACTION_H__)