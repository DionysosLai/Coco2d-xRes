/**
*@file ParticalNodeNode.cpp
*@brief 
*
*ÏêÏ¸¸ÅÊö
*
*@author DionysosLai£¬email: 906391500@qq.com
*@version 1.0
*@data 2014-7-1 12:04
*/
#include "ParticalNode.h"

USING_NS_CC;


ParticalNode::ParticalNode()
{

}

ParticalNode::~ParticalNode()
{

}

ParticalNode* ParticalNode::create( const char *plistFile )
{
	ParticalNode* pRet = new ParticalNode();
	if (pRet && pRet->init(plistFile))
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

bool ParticalNode::init( const char *plistFile )
{
	if (!CCNode::init())
	{
		return false;
	}

	initData(plistFile);

	return true;
}

void ParticalNode::initData( const char *plistFile )
{
	m_pPartical = CCParticleSystemQuad::create(plistFile);
	m_pPartical->setPosition(CCPointZero);//ÉèÖÃ·¢ÉäÁ£×ÓµÄÎ»ÖÃ
	this->addChild(m_pPartical);

	this->scheduleOnce(schedule_selector(ParticalNode::deletePartical), m_pPartical->getLife()+2.f);
}

void ParticalNode::deletePartical( float delta )
{
	this->removeChild(m_pPartical, true);
}

