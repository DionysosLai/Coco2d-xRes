/**
*@file FBStartScene.cpp
*@brief 最爱积木开始界面实现部分
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-6-27 17:11
*/
#include "FBStartScene.h"
#include "FBGameScene.h"
#include "AimScene.h"

#include "FBGameMusic.h"
#include "FBGamePigConst.h"

#include "../XtcUtils/XtcUtils.h"
#include "../XtcUtils/XtcOkiiRank.h"

#include "SimpleAudioEngine.h"

/*#include <vld.h>*/

USING_NS_CC;
using namespace cocos2d::extension;
using namespace CocosDenshion;

CCScene* StartScene::scene()
{
	CCScene *scene = CCScene::create();
	StartScene *layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}


bool StartScene::init()
{

	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

// 	int *p = new int(0x12345678);
// 	CCLOG("p=%08x, ", p);

	return true;
}

void StartScene::onEnter()
{
	CCLayer::onEnter();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	srand(time(0));

	this->scheduleUpdate();
	this->setKeypadEnabled(true);
	
	m_sPlistFileth = 0;		///< 开始时，加载第一个plist文件
	this->schedule(schedule_selector(StartScene::loadPlistFile), 0.05f);

	///
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	SimpleAudioEngine::sharedEngine()->stopVoice();
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	setFunBK();

	m_bTouch = false;
	m_isExitState = false;
}

void StartScene::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	if (NULL == m_actionAimBtn)
	{
		m_actionAimBtn->release();
		m_actionGameBtn->release();
	}
}

void StartScene::update( float delta )
{
}

bool StartScene::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint point = pTouch->getLocation();


	/// 判断点在哪里；
	if (!m_bTouch && ccpDistance(ccp(660.5, 200), point) < 150.f && 0 == m_spGameStartBtn->numberOfRunningActions())
	{
		m_bTouch = true;
		this->setKeypadEnabled(false);
		this->setTouchEnabled(false);
		m_spGameStartBtn->stopAllActions();
		m_spGameStartBtn->runAction((CCActionInterval*)m_actionGameBtn->copy()->autorelease());

		SimpleAudioEngine::sharedEngine()->playEffect(FB_ME_GMME);
		return true;
	}

	/// 判断点在哪里；
	if (!m_bTouch && ccpDistance(ccp(1178, 738.5), point) < 65.f && 0 == m_spAimStartBtn->numberOfRunningActions())
	{
		m_bTouch = true;
		this->setKeypadEnabled(false);
		this->setTouchEnabled(false);
		SimpleAudioEngine::sharedEngine()->playEffect(FB_ME_AIM);
		m_spAimStartBtn->stopAllActions();
		m_spAimStartBtn->runAction((CCActionInterval*)m_actionAimBtn->copy()->autorelease());
		return true;
	}
	return false;
}

void StartScene::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{

}

void StartScene::keyBackClicked()
{
	if (XtcUtils::isVideoPlaying())///< 如果视频正在播放，就关闭视频；
	{
		stopAllActions();
		XtcUtils::stopVideo();
		CCScene* pNewScene = StartScene::scene();
		CCDirector::sharedDirector()->replaceScene(pNewScene);
		setKeypadEnabled(false);
	}
	else if (m_isExitState == false)
	{
		m_isExitState = true;
		/// 委托触摸协议
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->removeDelegate(this);
		m_colorlayer = CCLayerColor::create(ccc4(0,0,0,110)); 
		addChild(m_colorlayer,255);
		CCSprite* exit_bg = CCSprite::create("Common/UI_exit_bg.png");
		exit_bg->setPosition(ccp(640,400));
		m_colorlayer->addChild(exit_bg);

		CCMenuItemImage* exit_confirm = CCMenuItemImage::create(
			"Common/UI_exit_confirm_normal.png",
			"Common/UI_exit_confirm_down.png",
			this,
			menu_selector(StartScene::onExitConfirm));
		exit_confirm->setPosition(ccp(-106.5,-84));

		CCMenuItemImage* exit_cancel = CCMenuItemImage::create(
			"Common/UI_exit_cancel_normal.png",
			"Common/UI_exit_cancel_down.png",
			this,
			menu_selector(StartScene::onExitCancel));
		exit_cancel->setPosition(ccp(111.5,-84));

		CCMenu* menu_exit = CCMenu::create(exit_cancel,exit_confirm,NULL);
		menu_exit->setPosition(CCPointZero + exit_bg->getTexture()->getContentSizeInPixels()/2);
		exit_bg->addChild(menu_exit);
	}
	else
	{
		onExitCancel(this);
	}
}

void StartScene::gameInto( float delta )
{
	CCScene* pScene = GameScene::scene();
	CCDirector* director = CCDirector::sharedDirector();
	director->replaceScene(pScene);
}

void StartScene::setFunBK()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	/// 背景
	CCTexture2DPixelFormat defaultFormat = CCTexture2D::defaultAlphaPixelFormat();
 	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
// 	CCTexture2D::setDefaultAlphaPixelFormat()
	CCSprite* bk = CCSprite::create("BJ_ZJM.png");
	bk->setPosition(ccp(visibleSize.width/2.f, visibleSize.height/2.f));
	this->addChild(bk, 0);
	
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
	/// 开始按钮
	m_spGameStartBtn = CCSprite::create("UI_play.png");
	m_spGameStartBtn->setPosition(ccp(616,306.5f));
	this->addChild(m_spGameStartBtn, 1);
	
	/// 家长站点
	m_spAimStartBtn = CCSprite::create("UI_JZZDH.png");
	m_spAimStartBtn->setPosition(ccp(1178, 738.5));
	this->addChild(m_spAimStartBtn, 1);

	CCScaleTo* pShrink = CCScaleTo::create(0.15f, 0.8f);
	CCScaleTo* pLargen = CCScaleTo::create(0.085f,1.15f);
	CCScaleTo* pReset = CCScaleTo::create(0.085f, 1.0f);
	m_actionGameBtn = CCSequence::create(pShrink, pLargen, pReset, 
		CCCallFunc::create(this, callfunc_selector(StartScene::onGameStartBtnSelected)), NULL);
	m_actionGameBtn->retain();

	m_actionAimBtn = CCSequence::create(pShrink, pLargen, pReset, 
		CCCallFunc::create(this, callfunc_selector(StartScene::onAimStartBtnSelected)), NULL);
	m_actionAimBtn->retain();

	/// 语音"比比蓝大冲关"
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(FB_M_TOPIC);
}

void StartScene::onGameStartBtnSelected()
{
	XtcUtils::setVideoDelayTime(200);
	XtcUtils::playVideo("video/ZAJM_PT.mp4");		
	this->scheduleOnce(schedule_selector(StartScene::gameInto), 0.5f);
}

void StartScene::onAimStartBtnSelected()
{
	/// 家长站点
	CCScene* scene = AimScene::scene();
	CCDirector* director = CCDirector::sharedDirector();
	director->replaceScene(scene);
}

void StartScene::onExitCancel( cocos2d::CCObject* pSender )
{
	removeChild(m_colorlayer);
	/// 委托触摸协议
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	m_isExitState = false;
}

void StartScene::onExitConfirm( cocos2d::CCObject* pSender )
{
	OkiiJni::okiiDailyExitTime();
	CCDirector::sharedDirector()->popScene();
}

void StartScene::loadPlistFile( float delta )
{
	/// 预加载	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(LG_FB_TP_PLIST[m_sPlistFileth], LG_FB_TP_PNG[m_sPlistFileth]);
	m_sPlistFileth++;

	if (LG_FB_TP_NUM <= m_sPlistFileth)
	{
		this->unschedule(schedule_selector(StartScene::loadPlistFile));
	}
}

