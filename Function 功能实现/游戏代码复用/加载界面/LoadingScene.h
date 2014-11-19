/**
*@file 
*@brief 
*
* 加载界面
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-10-15 15:43
*/

#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "XTC_Loading.h"

class LoadingScene : public cocos2d::CCLayer
{
public:
	virtual bool init();  
	static cocos2d::CCScene* scene();
	CREATE_FUNC(LoadingScene);

public:

	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	virtual void update(float delta);

///@brief 显示加载界面
///@param[in] delta----时间
///@return 
	void showLoadAnimation(float delta);

///@brief logo界面加载一些动画（不能再onEnter调用）
///@param[in] delta----时间
///@return 
	void loadSomeAnimations(float delta);


private:
	cocos2d::extension::CCArmature* m_arLoading;
	XTC_LoadingData* m_loadResNode;
	cocos2d::CCSprite* m_spLogo;		///<
};
#endif	///< (__LOADING_SCENE_H__)