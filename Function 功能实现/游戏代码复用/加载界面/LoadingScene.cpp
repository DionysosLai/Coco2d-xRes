/**
*@file 
*@brief 
*
*详细概述
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-10-15 15:47
*/
#include "LoadingScene.h"

#include "FBGameConst.h"
#include "FBGamePigConst.h"
#include "FBGameMusic.h"

#include "FBStartScene.h"

USING_NS_CC;
USING_NS_CC_EXT;


CCScene* LoadingScene::scene()
{
	CCScene *scene = CCScene::create();

	LoadingScene *layer = LoadingScene::create();

	scene->addChild(layer);

	return scene;
}


bool LoadingScene::init()
{

	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCLayerColor* colorLayer = CCLayerColor::create(ccc4(255, 255, 255, 255));
	this->addChild(colorLayer);

	m_spLogo = CCSprite::create("logo.png");
	m_spLogo->setPosition(ccp(640, 400));
	this->addChild(m_spLogo);

	return true;
}

void LoadingScene::onEnter()
{
	CCLayer::onEnter();
	srand(time(0));

}

void LoadingScene::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();

	this->scheduleOnce(schedule_selector(LoadingScene::loadSomeAnimations), 0.05f);

	this->scheduleOnce(schedule_selector(LoadingScene::showLoadAnimation), 2.0f);
}

void LoadingScene::onExit()
{
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	/// 清楚内存
	CCTextureCache::sharedTextureCache()->removeTextureForKey("logo.png");		///< 清楚动画信息

	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey("LoadingAnimation/DH_loading10.png"); 
	CCTextureCache::sharedTextureCache()->removeTexture(texture); 
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("LoadingAnimation/DH_loading10.plist"); 
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();

	CCLayer::onExit();
}

void LoadingScene::loadSomeAnimations( float delta )
{
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("LoadingAnimation/DH_loading1.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("DH_jmtx/DH_jmtx.ExportJson");
}

void LoadingScene::update( float delta )
{
	/// 判断是否加载完毕---ok 跳转到主界面
	if(m_loadResNode->getIsEndLoad())
	{
		this->unscheduleUpdate();
		CCScene* scene = StartScene::scene();
		CCDirector::sharedDirector()->replaceScene(scene);
	}
}

void LoadingScene::showLoadAnimation( float delta )
{

#if 1
	this->removeChild(m_spLogo, true);
	/// 显示加载界面
	CCArmature* loadingAnimate = CCArmature::create("DH_loading1");
	loadingAnimate->getAnimation()->play("loading");
	loadingAnimate->setPosition(ccp(620, 400));
	this->addChild(loadingAnimate);


	m_loadResNode = XTC_LoadingData::createLoadNode();		///< 能不用就不用----严重加长了loading时间 
	for (unsigned int i = 0; i < LG_FB_ARMATURE_NUM; i++)	///< 动画
	{
		m_loadResNode->addResource(LG_FB_ARMATURE[i]);
	}
	m_loadResNode->startLoading();
	this->addChild(m_loadResNode);

	this->scheduleUpdate();
#else
	CCScene* scene = StartScene::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
#endif

}


