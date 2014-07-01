/**
*@file S2CSprite.h
*@brief 实现精灵对象从矩形到圆形间互相转换
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-5-26 16:14
*/

#ifndef __S2C_SPRITE_H__
#define __S2C_SPRITE_H__

#include "cocos2d.h"

class S2CSprite : public cocos2d::CCSprite
{
public:  
	S2CSprite();  
	~S2CSprite();  

	static S2CSprite* create(const char* squareName, const char* circleName, float duration);  
	virtual bool init(const char* squareName, const char* circleName, float duration);  

private:
	enum
	{
		TAGSPRITE,
	};
private:  
	bool m_bShape;		///< true---矩形 false---圆形  
	cocos2d::CCActionInterval* m_circleAction;		///< 转化成圆形动作
	cocos2d::CCActionInterval* m_squareAction;		///< 转化成圆形动作
	cocos2d::CCTexture2D* m_textureSquare;			///< 矩形纹理；
	cocos2d::CCTexture2D* m_textureCircle;			///< 圆形纹理

	void initData(const char* squareName, const char* circleName, float duration);  


public:  
	void changeShape(); 
	void change();
};
#endif // (__S2C_SPRITE_H__)