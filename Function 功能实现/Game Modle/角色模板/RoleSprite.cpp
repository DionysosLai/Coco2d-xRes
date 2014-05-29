/**
*@file RoleSprite.cpp
*@brief 游戏角色处理
*@author DionysosLai，906391500@qq.com
*@version 1.0(2014-04-16)
*@data 2014-04-16
*/

#include "RoleSprite.h"

RoleSprite::RoleSprite()
{

}

RoleSprite::~RoleSprite()
{

}

RoleSprite* RoleSprite::create()
{
	RoleSprite *pSprite = new RoleSprite();  
	if (pSprite && pSprite->init())  
	{  
		pSprite->autorelease();  
		return pSprite;  
	}  
	CC_SAFE_DELETE(pSprite);  
	return NULL;  
}

bool RoleSprite::init()
{
	if (!CCSprite::init())  
	{  
		return false;  
	}  
	initData();  
	return true; 
}

void RoleSprite::initData()
{

}

///@brief 从一张png大图中创建动画
///@param[in] unitFrameTime--每一帧时间长度， plistName---plist文件名，png---png图片名， pngName---png图片里面的小名， frames---帧多少
///@pre 每一帧大小一致
///@return 
///@retval 
///@post 
///@author DionysosLai，906391500@qq.com
///@version 1.0(2014-04-16)
///@data 2014-04-16
CCAnimate* RoleSprite::createActionWithPng( float unitFrameTime, const char* pngName, CCSize frameSize, int frames )
{
	CCTexture2D *playerRunTexture = CCTextureCache::sharedTextureCache()->addImage(pngName);      
	CCAnimation* animation = CCAnimation::create();  
	for( int i = 0;i < frames; i++)
	{  
		animation->addSpriteFrame(CCSpriteFrame::createWithTexture(playerRunTexture, cocos2d::CCRectMake(frameSize.width*i, 0, frameSize.width, frameSize.height)));  
	}  
	// should last 2.8 seconds. And there are 14 frames.  
	animation->setDelayPerUnit(unitFrameTime);  
//	return animation;
	CCAnimate* action = CCAnimate::create(animation); 
	return action;
/*	return CCRepeatForever::create(action);  */
}

///@brief 从plist中创建动画，
///@param[in] unitFrameTime--每一帧时间长度， pngName---png图片名 frameSize---图片中每一帧大小， frames---帧多少
///@pre 注意这里的pngName，比较特殊，例如原名为“hatch_open_0.png”，这里必须传进来为hatch_open_0
///@return 
///@retval 
///@post 
///@author DionysosLai，906391500@qq.com
///@version 1.0(2014-04-16)
///@data 2014-04-16
CCAnimate* RoleSprite::createActionWithPlist(float unitFrameTime, const char* plistName, const char* png, const char* pngName, int frames)
{
    //可以在程序载入的时候预加载这些动画资源，然后在cache中读取 
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache(); 
    cache->addSpriteFramesWithFile(plistName, png);  

    CCArray* animFrames = CCArray::createWithCapacity(frames);
      
    char ch[64] = {0};  
    for(int i = 0; i < frames; i++)   
    {  
        sprintf(ch, "%d.png", i);  
		char str[64] = {0};	
		strcat(str, pngName);
		strcat(str, ch);
        CCSpriteFrame* frame = cache->spriteFrameByName( str );  
        animFrames->addObject(frame);  
    }  
      
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames,unitFrameTime);  
//	return animation;
	CCAnimate* action = CCAnimate::create(animation); 	
/*	return CCRepeatForever::create(CCAnimate::create(animation)); */
	return action;
}

///@brief 从多张图片png图片中，创建动画
///@param[in] unitFrameTime--每一帧时间长度，pngName---png图片名， frames---帧多少
///@pre 注意这里的pngName，比较特殊，例如原名为“hatch_open_0.png”，这里必须传进来为hatch_open_0
///@return 
///@retval 这中方法比较适合动画有位移
///@post 
///@author DionysosLai，906391500@qq.com
///@version 1.0(2014-04-16)
///@data 2014-04-16
CCAnimate* RoleSprite::createActoinWithPngs( float unitFrameTime, const char* pngName, int frames )
{
/*	CCTexture2D *playerRunTexture = CCTextureCache::sharedTextureCache()->addImage(pngName);*/      
	CCAnimation* animation = CCAnimation::create();  
	    
	char ch[64] = {0};
	for( int i = 0;i < frames; i++)
	{  
        sprintf(ch, "%d.png", i);
		char str[64] = {0};	
		strcat(str, pngName);
		strcat(str, ch);
		CCTexture2D *playerRunTexture = CCTextureCache::sharedTextureCache()->addImage(str);
		animation->addSpriteFrame(CCSpriteFrame::createWithTexture(playerRunTexture, 
			CCRectMake(0, 0, playerRunTexture->getContentSize().width, playerRunTexture->getContentSize().height)));  
	}  
 
	animation->setDelayPerUnit(unitFrameTime);  
//	return animation;
	CCAnimate* action = CCAnimate::create(animation);  
	return action;
/*	return CCRepeatForever::create(action);  */
}
///@brief 从cocostudio工具中创建动画
///@param[in] plistName--plist文件名，pngName---png图片名， josnName---josn文件名，armatureName--工程名，index---动作序列号
///@pre 
///@return 
///@retval
///@post 
///@author DionysosLai，906391500@qq.com
///@version 1.0(2014-04-16)
///@data 2014-04-16
CCArmature* RoleSprite::createArmature( const char* plistName, const char* pngName, const char* josnName, const char* armatureName, const unsigned int index )
{
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(pngName,plistName,josnName);
	CCArmature *armature = CCArmature::create(armatureName);
	armature->getAnimation()->playByIndex(0);
	
	return armature;
}

///@brief 返回当前的动画帧
///@param[in] animate---当前动画
///@pre	  这个函数的方法是通过获取当前纹理ID来实现，因此，使用范围是当前纹理ID都不一样，因此如果从一张大图中根据位置抽取动画数据，不可用。
///@return currentAnimIndex---当前动画帧 -1---获取失败
///@retval
///@post 
///@author DionysosLai，906391500@qq.com
///@version 1.0(2014-04-16)
///@data 2014-04-16
int RoleSprite::getCurrentFrameWithPngs( const CCAnimate* animate)
{
	int currentAnimIndex = 0; 
	unsigned int frameSum = animate->getAnimation()->getFrames()->capacity();	/// 获取动作的总帧数
	for(unsigned int i = 0; i < frameSum; i++)  
	{  
		GLint iID = this->getTexture()->getName();		///< 获取精灵当前纹理ID
		/// 依次获取动画帧，根据动画帧获取其纹理ID
		CCAnimationFrame* animFrame =(CCAnimationFrame*)animate->getAnimation()->getFrames()->objectAtIndex(i);		///< 注意这里是	CCAnimationFrame而不是CCSpriteFrame
		GLint iID1 = animFrame->getSpriteFrame()->getTexture()->getName();
		if (iID1 == iID)
		{
			currentAnimIndex = i;
			return currentAnimIndex;
		}
	}  
	return -1;
}

int RoleSprite::getCurrentFrameWithPng( const CCAnimate* animate)
{
// 	int currentAnimIndex = 0; //精灵当前播放的是第几帧  
// 	unsigned int frameSum = animate->getAnimation()->getFrames()->capacity();
// 	for(unsigned int i = 0; i < frameSum; i++)  
// 	{  
// 		CCRect rect = this->getTextureRect();
// // 		CCAnimationFrame* animFrame =(CCAnimationFrame*)animate->getAnimation()->getFrames()->objectAtIndex(i);
// // 		GLint iID1 = animFrame->getSpriteFrame()->getTexture()->getName();
// // 		if (iID1 == iID)
// 		{
// // 			currentAnimIndex = i;
// // 			return currentAnimIndex;
// // 		}		i = i;
// 	}  
 	return -1;
}


