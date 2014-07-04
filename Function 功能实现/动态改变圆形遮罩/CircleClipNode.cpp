/**
*@file CircleClipNode.cpp
*@brief 实现圆形遮罩效果 具体实现方式
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-5-30 10:40
*/
#include "CircleClipNode.h"

USING_NS_CC;


CircleClipNode::CircleClipNode()
{

}

CircleClipNode::~CircleClipNode()
{
/*	pClip->release();*/
}

///@brief 创建动态圆心遮罩
///@param[in]  cen--圆心 radius--半径 radiusDelta--- 每次半径渐变大小， radiusEnd--当半径大于或小于某一值时，设置运动结束 bInverted--true--显示“底板-模板内容 ”
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-5-30 11:14
CircleClipNode* CircleClipNode::create( cocos2d::CCPoint cen, float radius, float radiusDelta, float radiusEnd, bool bInverted )
{
	CircleClipNode *pRet = new CircleClipNode();
	if (pRet && pRet->init(cen, radius, radiusDelta, radiusEnd, bInverted))
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
}

bool CircleClipNode::init( cocos2d::CCPoint cen, float radius, float radiusDelta, float radiusEnd, bool bInverted )
{
	if (!CCNode::init())  
	{  
		return false;  
	}  

	initData(cen, radius, radiusDelta, radiusEnd, bInverted);  
	return true; 
}

void CircleClipNode::initData( cocos2d::CCPoint cen, float radius, float radiusDelta, float radiusEnd, bool bInverted )
{
	m_fRadius		= radius;
	m_pointCen		= cen;
	m_fRadiusDelta	= radiusDelta;
	m_fRadiusEnd	= radiusEnd;
	m_bStartClip	= false;
	m_bRunClip		= false;
	m_uiRunclip		= 0.f;
	//创建cliper
	CCClippingNode* pClip=CCClippingNode::create();
	pClip->setInverted(bInverted);
	addChild(pClip, 0, TAGCLIPNODE);
	//添加底板
	CCLayerColor* pColor=CCLayerColor::create(ccc4(0,0,0,200));
	pClip->addChild(pColor);

	//绘制圆形区域
	static ccColor4F green	= {0, 1, 0, 1};					///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	const int nCount		= 100;							///< 圆形其实可以看做正多边形,我们这里用正100边型来模拟园
	const float coef		= 2.0f * (float)M_PI/nCount;	///< 计算每两个相邻顶点与中心的夹角
	static CCPoint circle[nCount];							///< 顶点数组
	for(unsigned int i = 0;i < nCount; ++i) 
	{
		float rads	= i*coef;//弧度
		circle[i].x = m_pointCen.x + m_fRadius * cosf(rads);//对应顶点的x
		circle[i].y = m_pointCen.y + m_fRadius * sinf(rads);//对应顶点的y
	}

	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(circle, nCount, green, 0, green);//绘制这个多边形!

	this->schedule(schedule_selector(CircleClipNode::onRunClip), 0.05f);
	//设这模板
	pClip->setStencil(pStencil);

	this->setEnd(false);
}

void CircleClipNode::update( float delta )
{
	do 
	{		
		if(!m_bStartClip)
		{
			break;			
		}	


		m_fRadius = m_fRadius + m_fRadiusDelta;


		/// 判断是否动态改变圆心遮罩 结束否
		if ( (m_fRadiusDelta > 0 && m_fRadius > m_fRadiusEnd) || 
			(m_fRadiusDelta < 0 && m_fRadius < m_fRadiusEnd))
		{
			setEnd(true);
			this->unscheduleUpdate();
			return;
		}

		//绘制圆形区域
		static ccColor4F green	= {0, 1, 0, 1};					///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
		const int nCount		= 100;							///< 圆形其实可以看做正多边形,我们这里用正100边型来模拟园
		const float coef		= 2.0f * (float)M_PI/nCount;	///< 计算每两个相邻顶点与中心的夹角
		static CCPoint circle[nCount];							///< 顶点数组
		for(unsigned int i = 0;i < nCount; ++i) 
		{
			float rads	= i*coef;//弧度
			circle[i].x = m_pointCen.x + m_fRadius * cosf(rads);//对应顶点的x
			circle[i].y = m_pointCen.y + m_fRadius * sinf(rads);//对应顶点的y
		}

		CCDrawNode *pStencil=CCDrawNode::create();
		pStencil->drawPolygon(circle, nCount, green, 0, green);//绘制这个多边形!

		//设这模板
		CCClippingNode* pClip = (CCClippingNode*)this->getChildByTag(TAGCLIPNODE);
		pClip->setStencil(pStencil);
	} while (0);

}

void CircleClipNode::setEnd(bool var)
{
	this->m_bEnd = var;
}

bool CircleClipNode::getEnd()
{
	return this->m_bEnd;
}

///@brief 开始运动
///@param[in/out] 
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-8 10:15
void CircleClipNode::startClip()
{
	m_bStartClip = true;
	this->unschedule(schedule_selector(CircleClipNode::onRunClip));
}

void CircleClipNode::onEnter()
{
	CCNode::onEnter();
	this->scheduleUpdate();
}

void CircleClipNode::onExit()
{
	CCNode::onExit();
}

void CircleClipNode::onRunClip( float delta )
{
	if (m_bRunClip)
	{
		m_fRadius += 2;
		m_uiRunclip++;
		if (m_uiRunclip > 10)
		{
			m_bRunClip = false;
			m_uiRunclip = 0;
		}

	}
	else
	{
		m_fRadius -= 2;
		m_uiRunclip++;
		if (m_uiRunclip > 10)
		{
			m_bRunClip = true;
			m_uiRunclip = 0;
		}
	}

	//绘制圆形区域
	static ccColor4F green	= {0, 1, 0, 1};					///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	const int nCount		= 100;							///< 圆形其实可以看做正多边形,我们这里用正100边型来模拟园
	const float coef		= 2.0f * (float)M_PI/nCount;	///< 计算每两个相邻顶点与中心的夹角
	static CCPoint circle[nCount];							///< 顶点数组
	for(unsigned int i = 0;i < nCount; ++i) 
	{
		float rads	= i*coef;//弧度
		circle[i].x = m_pointCen.x + m_fRadius * cosf(rads);//对应顶点的x
		circle[i].y = m_pointCen.y + m_fRadius * sinf(rads);//对应顶点的y
	}

	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(circle, nCount, green, 0, green);//绘制这个多边形!

	//设这模板
	CCClippingNode* pClip = (CCClippingNode*)this->getChildByTag(TAGCLIPNODE);
	pClip->setStencil(pStencil);
}
