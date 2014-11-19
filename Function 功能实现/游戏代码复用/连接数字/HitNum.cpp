/**
*@file HitNum.cpp
*@brief 连击数字效果
*
*详细概述
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-10-23 11:37
*/
#include "HitNum.h"
#include "FBGamePigConst.h"

USING_NS_CC;


HitNum::HitNum()
{

}

HitNum::~HitNum()
{

}

HitNum* HitNum::create( const int& num )
{
	HitNum* pRet = new HitNum();
	if (pRet && pRet->init(num))
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

bool HitNum::init( const int& num )
{
	if (!CCNode::init())
	{
		return false;
	}

	initData(num);

	return true;
}

void HitNum::initData( const int& num )
{
	/// 第一个数字为“X”
	CCSprite* sp1 = CCSprite::createWithSpriteFrameName(LG_FB_PIG_NUM[10]);
	sp1->setPosition(CCPointZero);
	this->addChild(sp1);

	CCActionInterval* moveby = CCMoveBy::create(1.5f, ccp(0, 200));
	CCActionInterval* fadeout = CCFadeOut::create(1.5f);

	CCActionInterval* action1 = CCSpawn::create(moveby, fadeout, NULL);
	CCActionInterval* action = CCSequence::create(action1, CCRemoveSelf::create(), NULL);

	sp1->runAction((CCActionInterval*)action->copy()->autorelease());

	/// 判断num大小---不可能出现大于100的
	if(num > 9)
	{
		CCSprite* sp2 = CCSprite::createWithSpriteFrameName(LG_FB_PIG_NUM[num/10]);
		sp2->setPosition(ccp(50, 0));
		this->addChild(sp2);
		CCSprite* sp3 = CCSprite::createWithSpriteFrameName(LG_FB_PIG_NUM[num%10]);
		sp3->setPosition(ccp(100, 0));
		this->addChild(sp3);

		sp2->runAction((CCActionInterval*)action->copy()->autorelease());
		sp3->runAction((CCActionInterval*)action->copy()->autorelease());

	}
	else
	{
		CCSprite* sp2 = CCSprite::createWithSpriteFrameName(LG_FB_PIG_NUM[num]);
		sp2->setPosition(ccp(50, 0));
		this->addChild(sp2);

		sp2->runAction((CCActionInterval*)action->copy()->autorelease());
	}

}
