/**
*@file RoleSprite.h
*@brief 游戏角色处理
*@author DionysosLai，906391500@qq.com
*@version 1.0(2014-04-16)
*@data 2014-04-16
*/
#ifndef __ROLE_SPRITE_H__
#define __ROLE_SPRITE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::gui;
using namespace cocos2d::extension;

class RoleSprite : public CCSprite
{
public:
	RoleSprite();
	virtual ~RoleSprite();

	static RoleSprite* create();
	virtual bool init(); 

private:  
	void initData(); 

public:
	CCAnimate*	createActionWithPng(float unitFrameTime, const char* pngName, CCSize frameSize, int frames);
	CCAnimate*	createActionWithPlist(float unitFrameTime, const char* plistName, const char* png, const char* pngName, int frames);
	CCAnimate*	createActoinWithPngs(float unitFrameTime, const char* pngName, int frames);
	CCArmature*	createArmature(const char* plistName, const char* pngName, const char* josnName, const char* armatureName, const unsigned int index);

	int	getCurrentFrameWithPngs(const CCAnimate* animation);
	int	getCurrentFrameWithPng(const CCAnimate* animation);
}; /* defined(__ROLE_SPRITE_H__) */  
#endif
