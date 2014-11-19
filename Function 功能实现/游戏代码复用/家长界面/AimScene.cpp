/**
*@file AimScene.cpp
*@brief 目的场景
*
*详细概述
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-6-27 17:50
*/
#include "AimScene.h"
#include "FBStartScene.h"

USING_NS_CC;

CCScene* AimScene::scene()
{
	CCScene *scene = CCScene::create();
	AimScene *layer = AimScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool AimScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCTexture2DPixelFormat defaultPixleFormat = CCTexture2D::defaultAlphaPixelFormat();
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	CCSprite* spBK = CCSprite::create("FB_JZJM.png");
	spBK->setPosition(ccp(visibleSize.width/2.f, visibleSize.height/2.f));
	this->addChild(spBK, -1);
	CCTexture2D::setDefaultAlphaPixelFormat(defaultPixleFormat);

	this->setKeypadEnabled(true);

	return true;
}
void AimScene::keyBackClicked()
{
	CCLOG("RecommendScene");

	/// 开始界面
	CCScene* scene = StartScene::scene();
	CCDirector* director = CCDirector::sharedDirector();
	director->replaceScene(scene);
}

void AimScene::onEnter()
{
	CCLayer::onEnter();
}

void AimScene::onExit()
{
	CCLayer::onExit();

	/// 清理缓存
	CCTextureCache::sharedTextureCache()->removeTextureForKey("FB_JZJM.png");
}
