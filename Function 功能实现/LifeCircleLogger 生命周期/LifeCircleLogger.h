#ifndef _LIFE_CIRCLELOGGER_H__
#define _LIFE_CIRCLELOGGER_H__

#include "cocos2d.h"

USING_NS_CC;

class LifeCircleLogger
{
private:
	CCString m_msg;
public:
	LifeCircleLogger();
	LifeCircleLogger(const CCString& msg):m_msg(msg)
	{
		CCLog("%s BEGINS!", m_msg.getCString());
	}
	~LifeCircleLogger()
	{
		CCLog("%s ENDS!", m_msg.getCString());
	}
};
#define LOG_FUNCTION_LIFE LifeCircleLogger(__FUNCTION__);

#endif

