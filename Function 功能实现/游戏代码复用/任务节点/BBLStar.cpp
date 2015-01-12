/**Copyright (c) 2014, 广东小天才科技有限公司
*All rights reserved.
*@file BBLStar.cpp
*@brief 星星节点
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-8-28 16:51
*/
#include "BBLStar.h"
#include "ParticalNode.h"
#include "GameMusicConst.h"
#include "SimpleAudioEngine.h"



USING_NS_CC;
using namespace CocosDenshion;

Star::Star()
{

}

Star::~Star()
{
	m_texttureL->release();
	m_texttureA->release();
	m_starArray->release();
}

Star* Star::create( const unsigned int& starNum )
{
	Star* pRet = new Star();
	if (pRet && pRet->init(starNum))
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

bool Star::init( const unsigned int& starNum )
{
	if (!CCNode::init())
	{
		return false;
	}

	initData(starNum);
	return true;
}

void Star::initData( const unsigned int& starNum )
{

	m_texttureL = CCTextureCache::sharedTextureCache()->addImage("png/PIC_UI_XX_L.png");
	m_texttureA = CCTextureCache::sharedTextureCache()->addImage("png/PIC_UI_XX_A.png");
	m_texttureL->retain();
	m_texttureA->retain();
	m_starArray = CCArray::createWithCapacity(starNum);
	for (unsigned int i = 0; i < m_starArray->capacity(); ++i)
	{
		CCSprite* star = CCSprite::createWithTexture(m_texttureA);
		CCPoint pos = ccp(1225, 747);
		pos.x = pos.x - 120*(starNum-i-1);
		star->setPosition(pos);
		this->addChild(star);
		m_starArray->addObject(star);
	}
	m_starArray->retain();
	m_uiTask = 0;
}

void Star::addTask()
{
	CCSprite* star = (CCSprite*)m_starArray->objectAtIndex(m_uiTask);
	star->initWithTexture(m_texttureL);


	ParticalNode* partical = ParticalNode::create("partical/fire.plist");
	CCPoint pos = ccp(1225, 747);
	pos.x = pos.x - 120*(m_starArray->capacity()-m_uiTask-1);
	partical->setPosition(pos);
	this->addChild(partical);
	m_uiTask++;
	SimpleAudioEngine::sharedEngine()->playEffect(BBL_ME_STARLIGHT);
}

void Star::reduceTask()
{
	m_uiTask--;
	CCSprite* star = (CCSprite*)m_starArray->objectAtIndex(m_uiTask);
	star->initWithTexture(m_texttureA);
}
