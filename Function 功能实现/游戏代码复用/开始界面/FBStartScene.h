/**
*@file FBStartScene.h
*@brief 最爱积木游戏开始界面
*
* 由于本游戏动画少，因此没有加载界面
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-10-1 15:08
*/

#ifndef __FB_START_SCENE_H__
#define __FB_START_SCENE_H__


#include "cocos2d.h"
#include "cocos-ext.h"

class StartScene : public cocos2d::CCLayer
{
public:
	virtual bool init();  
	static cocos2d::CCScene* scene();
	CREATE_FUNC(StartScene);

	static bool s_isResourceLoaded;
public:

	virtual void onEnter();
	virtual void onExit();

	virtual void update(float delta);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void keyBackClicked();

	void gameInto(float delta);

///@brief 主界面---加载完毕显示
///@param[] 
///@pre 前者条件 
///@return
	void setFunBK();

///@brief 游戏开始按钮动作回调函数
///@param[in/out] 
///@pre 前者条件 
///@return 
	void onGameStartBtnSelected();

///@brief 家长站点按钮动作回调函数
///@param[in/out] 
///@pre 前者条件 
///@return 
	void onAimStartBtnSelected();

///@brief 加载tp打包的plist文件----理论上不能同时加载文件，之间至少间隔1帧
///@param[in] delta---间隔时间 
///@return 
	void loadPlistFile(float delta);

	void onExitCancel(cocos2d::CCObject* pSender);

	void onExitConfirm(cocos2d::CCObject* pSender);

private:
	bool m_bTouch;
	cocos2d::CCSprite* m_spGameStartBtn;			///< 游戏开始按钮
	cocos2d::CCSprite* m_spAimStartBtn;				///< 家长站点
	cocos2d::CCActionInterval* m_actionAimBtn;		///< 家长站点按钮点击动画
	cocos2d::CCActionInterval* m_actionGameBtn;		///< 游戏按钮点击动画

	bool m_isExitState;								///< 退出状态
	cocos2d::CCLayerColor* m_colorlayer;

	unsigned short m_sPlistFileth;					///< 加载第几个plist文件
};
#endif ///< (__FB_START_SCENE_H__)