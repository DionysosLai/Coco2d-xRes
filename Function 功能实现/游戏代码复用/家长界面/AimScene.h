/**
*@file AimScene.h
*@brief 目的场景
*
*详细概述
*
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-6-27 17:50
*/

#ifndef __SDM_AIM_SCENE_H__
#define __SDM_AIM_SCENE_H__

#include "cocos2d.h"

class AimScene : public cocos2d::CCLayer
{
public:
	virtual bool init();  

	static cocos2d::CCScene* scene();

	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(AimScene);

	virtual void keyBackClicked();
};

#endif // __SDM_AIM_SCENE_H__