/**Copyright (c) 2014, 广东小天才科技有限公司
*All rights reserved.
*@file BBLStar.h
*@brief 星星节点
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-8-28 16:47
*/

#ifndef __BBL_STAR_H__
#define __BBL_STAR_H__

#include "cocos2d.h"

class Star : public cocos2d::CCNode
{
public:
	Star();
	~Star();

	static Star* create(const unsigned int& starNum);
	virtual bool init(const unsigned int& starNum);

public:
///@brief 增加一个目标数
///@return 
	void addTask();
///@brief 减少一个目标数
///@return 
	void reduceTask();
private:
	void initData(const unsigned int& starNum);
private:
	cocos2d::CCArray* m_starArray;
	unsigned int m_uiTask;					///< 完成的目标数量

	cocos2d::CCTexture2D* m_texttureL;		///< 星星点亮纹理
	cocos2d::CCTexture2D* m_texttureA;		///< 星星暗纹理
};

#endif