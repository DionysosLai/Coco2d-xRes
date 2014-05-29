#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "cocos2d.h"


USING_NS_CC;

class GameMenu : public CCMenu
{
public:
	/// 构造
	GameMenu(void);
	~GameMenu(void);
	/// 重载
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	/// 
	GameMenu* menuWithItems(CCMenuItem* item, ...);
	/// 菜单是否可见
	bool getIsVisible(void);
	/// 设置菜单是否可见
	void setIsVisible(bool isVisible);

private:
	CCMenuItem* m_pSelectedItem;
	bool	m_bIsVisible;		///< 菜单是否可见;
};
#endif