/**
*@file HelpPrompt.cpp
*@brief 新手提示
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-11-4 16:07
*/
#include "HelpPrompt.h"

USING_NS_CC;

enum 
{
	TAGCLIP,
};

HelpPrompt::HelpPrompt()
{

}

HelpPrompt::~HelpPrompt()
{

}

HelpPrompt* HelpPrompt::Create( const cocos2d::CCPoint& pos )
{
	HelpPrompt* pRet = new HelpPrompt();
	if (pRet && pRet->init(pos))
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

bool HelpPrompt::init( const cocos2d::CCPoint& pos )
{
	if (!CCLayer::init())
	{
		return false;
	}

	initData(pos);

	return true;
}

void HelpPrompt::initData( const cocos2d::CCPoint& pos )
{
	m_sPromptType	= HELP_PROMPT_E_RECTANGLE;
	m_fWidth		= 100.f;
	m_fHeight		= 100.f;
	m_bSwallow		= false;
	CCPoint posParent = this->getPosition();		///< get parent location;
	m_ptLocation = ccpSub(pos, posParent);

	//创建cliper
	CCClippingNode* pClip=CCClippingNode::create();
	pClip->setInverted(true);
	addChild(pClip, 0, TAGCLIP);
	//添加底板
	CCLayerColor* pColor=CCLayerColor::create(ccc4(0,0,0,125));
	pClip->addChild(pColor);

	//绘制长方形区域
	static ccColor4F white	= {0,0,0,255};					///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	static CCPoint rectangle[4];							///< 顶点数组
	rectangle[0] = ccp(m_ptLocation.x-50.f, m_ptLocation.y+50.f);		///< leftup point
	rectangle[1] = ccp(m_ptLocation.x-50.f, m_ptLocation.y-50.f);		///< leftbuttom point
	rectangle[2] = ccp(m_ptLocation.x+50.f, m_ptLocation.y-50.f);		///< rightbuttom point
	rectangle[3] = ccp(m_ptLocation.x+50.f, m_ptLocation.y+50.f);		///< rightup point

	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(rectangle, 4, white, 0, white);//绘制这个多边形!

	//设这模板
	pClip->setStencil(pStencil);
}

void HelpPrompt::onEnter()
{
//	this->setTouchEnabled(true);
	CCLayer::onEnter();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -128, false);		///< 最高响应，并且不吞掉触摸


}

void HelpPrompt::onExit()
{
	CCLayer::onExit();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
}

void HelpPrompt::setPromptType(unsigned short var)
{
	CCAssert(HELP_PROMPT_E_RECTANGLE == var || HELP_PROMPT_E_CIRCLE == var, "There is no other prompt type!");

	m_sPromptType = var;
}

unsigned short HelpPrompt::getPromptType()
{
	return this->m_sPromptType;
}

void HelpPrompt::setWidth(float var)
{
	CCAssert(HELP_PROMPT_E_RECTANGLE == m_sPromptType, "Please be sure that the prompt type is HELP_PROMPT_E_RECTANGLE!");

	m_fWidth = var;

	//绘制长方形区域
	static ccColor4F white	= {0,0,0,255};					///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	static CCPoint rectangle[4];							///< 顶点数组
	rectangle[0] = ccp(m_ptLocation.x-m_fWidth/2.f, m_ptLocation.y+m_fHeight/2.f);		///< leftup point
	rectangle[1] = ccp(m_ptLocation.x-m_fWidth/2.f, m_ptLocation.y-m_fHeight/2.f);		///< leftbuttom point
	rectangle[2] = ccp(m_ptLocation.x+m_fWidth/2.f, m_ptLocation.y-m_fHeight/2.f);		///< rightbuttom point
	rectangle[3] = ccp(m_ptLocation.x+m_fWidth/2.f, m_ptLocation.y+m_fHeight/2.f);		///< rightup point

	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(rectangle, 4, white, 0, white);//绘制这个多边形!

	//设这模板
	CCClippingNode* pClip = (CCClippingNode*)this->getChildByTag(TAGCLIP);
	pClip->setStencil(pStencil);
}

float HelpPrompt::getWidth()
{
	return this->m_fWidth;
}

void HelpPrompt::setHeight(float var)
{
	CCAssert(HELP_PROMPT_E_RECTANGLE == m_sPromptType, "Please be sure that the prompt type is HELP_PROMPT_E_RECTANGLE!");

	m_fHeight = var;

	//绘制长方形区域
	static ccColor4F white	= {0,0,0,255};				///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	static CCPoint rectangle[4];							///< 顶点数组
	rectangle[0] = ccp(m_ptLocation.x-m_fWidth/2.f, m_ptLocation.y+m_fHeight/2.f);		///< leftup point
	rectangle[1] = ccp(m_ptLocation.x-m_fWidth/2.f, m_ptLocation.y-m_fHeight/2.f);		///< leftbuttom point
	rectangle[2] = ccp(m_ptLocation.x+m_fWidth/2.f, m_ptLocation.y-m_fHeight/2.f);		///< rightbuttom point
	rectangle[3] = ccp(m_ptLocation.x+m_fWidth/2.f, m_ptLocation.y+m_fHeight/2.f);		///< rightup point

	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(rectangle, 4, white, 0, white);//绘制这个多边形!

	//设这模板
	CCClippingNode* pClip = (CCClippingNode*)this->getChildByTag(TAGCLIP);
	pClip->setStencil(pStencil);
}

float HelpPrompt::getHeight()
{
	return m_fHeight;
}

void HelpPrompt::setRadius(float var)
{
	CCAssert(HELP_PROMPT_E_CIRCLE == m_sPromptType, "Please be sure that the prompt type is HELP_PROMPT_E_CIRCLE!");

	m_fRadius = var;
}

float HelpPrompt::getRadius()
{
	return this->m_fRadius;
}

void HelpPrompt::setLocation(cocos2d::CCPoint var)
{
	CCPoint posParent = this->getPosition();		///< get parent location;
	m_ptLocation = ccpSub(var, posParent);

	//绘制长方形区域
	static ccColor4F white	= {0,0,0,255};					///< 顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	static CCPoint rectangle[4];							///< 顶点数组
	rectangle[0] = ccp(m_ptLocation.x-m_fWidth/2.f, m_ptLocation.y+m_fHeight/2.f);		///< leftup point
	rectangle[1] = ccp(m_ptLocation.x-m_fWidth/2.f, m_ptLocation.y-m_fHeight/2.f);		///< leftbuttom point
	rectangle[2] = ccp(m_ptLocation.x+m_fWidth/2.f, m_ptLocation.y-m_fHeight/2.f);		///< rightbuttom point
	rectangle[3] = ccp(m_ptLocation.x+m_fWidth/2.f, m_ptLocation.y+m_fHeight/2.f);		///< rightup point

	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(rectangle, 4, white, 0, white);//绘制这个多边形!

	//设这模板
	CCClippingNode* pClip = (CCClippingNode*)this->getChildByTag(TAGCLIP);
	pClip->setStencil(pStencil);
}

cocos2d::CCPoint HelpPrompt::getLocation()
{
	CCPoint posParent = this->getPosition();		///< get parent location;
	
	return ccpAdd(posParent, m_ptLocation);
}

void HelpPrompt::setSwallow(bool var)
{
	this->m_bSwallow = var;
}

bool HelpPrompt::getSwallow()
{
	return m_bSwallow;
}

bool HelpPrompt::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint pos = pTouch->getLocation();

	this->setSwallow(bSwallowTouch(pos));		///< set m_bSwallow property!

	return true;
}

void HelpPrompt::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint pos = pTouch->getLocation();

	this->setSwallow(bSwallowTouch(pos));		///< set m_bSwallow property!
}


void HelpPrompt::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint pos = pTouch->getLocation();

	this->setSwallow(bSwallowTouch(pos));		///< set m_bSwallow property!
}


bool HelpPrompt::bSwallowTouch( const cocos2d::CCPoint& pos )
{
	bool bSwallow = false;
	if (HELP_PROMPT_E_RECTANGLE == m_sPromptType)
	{
		/// 判断点是否在长方形内
		if (pos.x >= m_ptLocation.x - m_fWidth/2.f && pos.x <= m_ptLocation.x + m_fWidth/2.f &&
			pos.y >= m_ptLocation.y - m_fHeight/2.f && pos.y <= m_ptLocation.y + m_fHeight/2.f)
		{
			bSwallow = false;
		}
		else
		{
			bSwallow = true;
		}
	}
	else if (HELP_PROMPT_E_CIRCLE == m_sPromptType)
	{
		/// 判断点是否在圆内
		if (ccpDistance(pos, m_ptLocation) <= m_fRadius)
		{
			bSwallow = false;
		}
		else
		{
			bSwallow = true;
		}
	}
	else
	{
		bSwallow = true;
	}

	return bSwallow;
}
