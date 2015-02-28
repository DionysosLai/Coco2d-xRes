/**
*@file TimeSchedule.cpp
*@brief 
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-6-17 15:57
*/
#include "TimeSchedule.h"
#include "ENVGameScene.h"

USING_NS_CC;

static const char* TIME_PIG_BUTTOM					= "Time/PIC_UI_SJJDT3.png";			///< 时间进度调底部
static const cocos2d::CCPoint TIME_POINT_BUTTOM		= ccp(0, 0);
static const char* TIME_PIG_UP						= "Time/PIC_UI_SJJDT2.png";			///< 时间进度调顶部
static const cocos2d::CCPoint TIME_POINT_UP			= ccp(-13, 1);
static const char* TIME_PIG_UP_RIGHT				= "Time/PIC_UI_SJJDT21.png";			///< 时间进度调顶部
static const cocos2d::CCPoint TIME_POINT_UP_RIGHT	= ccp(142, 1);
static const char* TIME_PIG_ALARM					= "Time/PIC_UI_SJJDT1.png";			///<装饰品---小闹钟
static const cocos2d::CCPoint TIME_POINT_ALARM		= ccp(-142, 0);

TimeSchedule::TimeSchedule()
{

}

TimeSchedule::~TimeSchedule()
{
	m_pText2d->release();
}

TimeSchedule* TimeSchedule::create(const unsigned int& totalTime)
{
	TimeSchedule* pRet = new TimeSchedule();
	if (pRet && pRet->init(totalTime))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
	return pRet;
}

bool TimeSchedule::init(const unsigned int& totalTime)
{
	if(!CCNode::init())
	{
		return false;
	}
	initData(totalTime);

	return true;
}

///@brief 进度条类初始化
///@param[]void
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-6-17 15:58
void TimeSchedule::initData(const unsigned int& totalTime)
{
	/// 底板
	m_pButtom = CCSprite::create(TIME_PIG_BUTTOM);
	m_pButtom->setPosition(TIME_POINT_BUTTOM);
	this->addChild(m_pButtom);

	m_pText2d = CCTextureCache::sharedTextureCache()->addImage(TIME_PIG_UP);
	m_pText2d->retain();

	m_pProgress = CCSprite::createWithTexture(m_pText2d);
	CCPoint pos = TIME_POINT_UP;
	pos.x -= m_pProgress->getContentSize().width/2.f;
	m_pProgress->setPosition(pos);
	m_pProgress->setAnchorPoint(ccp(0, 0.5f));
//	m_pProgress->setScaleX(-1.f);
	m_fTimeLength = m_pProgress->getContentSize().width;
	this->addChild(m_pProgress);

	/// 进度条右部
	m_pProgressRight = CCSprite::create(TIME_PIG_UP_RIGHT);
	m_pProgressRight->setPosition(TIME_POINT_UP_RIGHT);
	this->addChild(m_pProgressRight);
	

	/// 小闹钟装饰
	CCSprite* m_spAlarm = CCSprite::create(TIME_PIG_ALARM);
	m_spAlarm->setPosition(TIME_POINT_ALARM);
//	m_spAlarm->setVisible(false);
	this->addChild(m_spAlarm);

	m_fCurrentTime = 0.f;
	m_fTotalTime = totalTime;
	m_fAddTime		= 0.f;
	m_fReduceTime	= 0.f;
}

///@brief 开始计时
///@param[void] void
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-6-17 18:01
void TimeSchedule::startTimeProgrss()
{
	this->scheduleUpdate();
}

///@brief 停止时间计时
///@param[void] void 
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-6-17 18:02
void TimeSchedule::stopTimeProgress()
{
	this->unscheduleUpdate();
}

///@brief 增加时间
///@param[in] time---增加的时间
///@pre 增加后的时间不能高于最高时间
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-6-17 18:02
void TimeSchedule::addTime( const unsigned int& time, const float& timeGap )
{
	this->unschedule(schedule_selector(TimeSchedule::addTimeSpeed));
	m_fAddTime += time;
	this->schedule(schedule_selector(TimeSchedule::addTimeSpeed), timeGap);
}

///@brief 控制增加时间速度
///@param[in/out] 
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-9 10:18
void TimeSchedule::addTimeSpeed( float delta )
{
	if (m_fAddTime < 0)
	{
		this->unschedule(schedule_selector(TimeSchedule::addTimeSpeed));
	}
	else
	{
		m_fCurrentTime -= 1.f;
		if (m_fCurrentTime < 0)
		{
			m_fCurrentTime = 0;
		}
		m_fAddTime -= 1.f;
	}
}

///@brief 控制减少时间速度
///@param[in/out] 
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-9 10:18
void TimeSchedule::reduceTimeSpeed( float delta )
{
	if (m_fReduceTime < 0)
	{
		this->unschedule(schedule_selector(TimeSchedule::addTimeSpeed));
	}
	else
	{
		m_fCurrentTime += 1.f;
		m_fReduceTime -= 1.f;
	}
}



///@brief 减少时间
///@param[in] time---减少的时间 
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-6-17 18:03
void TimeSchedule::reduceTime( const unsigned int& time, const float& timeGap )
{
	this->unschedule(schedule_selector(TimeSchedule::reduceTimeSpeed));
	m_fReduceTime += time;
	this->schedule(schedule_selector(TimeSchedule::reduceTimeSpeed), timeGap);
}

///@brief 时间流速调整
///@param[in] times---倍数  timeLast
///@pre times大于0---时间加速消失， 小于0---即时间， -1--时间静止
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-6-17 18:03
void TimeSchedule::adjustTime( const float& times, unsigned int& timeLast )
{

}

void TimeSchedule::update( float delta )
{
	m_fCurrentTime = m_fCurrentTime + delta;
	if (m_fTotalTime  < m_fCurrentTime)
	{
		/// 游戏结束
		this->unscheduleUpdate();
/*		CCLOG("Game Over!");*/
		GameScene* gameScene = (GameScene*)this->getParent();
		gameScene->gameEnd();
	}
	{
		m_pProgress->setTextureRect(CCRectMake(0, 0, m_fTimeLength * (m_fTotalTime-m_fCurrentTime)/m_fTotalTime, m_pText2d->getContentSize().height));
		CCPoint pos = m_pProgressRight->getPosition();
		pos.x -= m_fTimeLength*delta/m_fTotalTime;
		m_pProgressRight->setPosition(pos);
	}
}

void TimeSchedule::resetTime( const float& timeGap )
{
	CCAssert(timeGap >= 0, "timeGap must be greater than 0!");
	this->unschedule(schedule_selector(TimeSchedule::addTimeSpeed));
	m_fAddTime += m_fTotalTime-m_fCurrentTime + timeGap;			///< 由于要花费timeGap时间去复位，因此这个时间要加回来
	this->schedule(schedule_selector(TimeSchedule::addTimeSpeed), timeGap);
}

void TimeSchedule::resetTime()
{
	resetTime(0);
}
