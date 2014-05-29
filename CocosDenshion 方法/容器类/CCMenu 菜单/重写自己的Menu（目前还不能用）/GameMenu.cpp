#include "GameMenu.h"

USING_NS_CC;

bool GameMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	/// 把传进来的这个参数强转下，免得编译器报参数未使用的警告。
	CC_UNUSED_PARAM(pEvent);  
	if (m_eState != kCCMenuStateWaiting || ! m_bIsVisible)  
	{  
		return false;  
	}  

	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())  
	{  
		if (c->getIsVisible() == false)  
		{  
			return false;  
		}  
	}  

	m_pSelectedItem = this->itemForTouch(pTouch);  
	if (m_pSelectedItem)  
	{  
		m_eState = kCCMenuStateTrackingTouch;  
		m_pSelectedItem->selected();  
		///这里加入自己想要的效果。。。。。。。。。。。。。。。。  
		return true;  
	}  
	return false;  
}

GameMenu* GameMenu::menuWithItems(CCMenuItem* item, ...)
{
	va_list args;  
	va_start(args,item);  
	GameMenu *pRet = new GameMenu();	///< 这里记得要注意  
	if (pRet && pRet->initWithItems(item, args))  
	{  
		pRet->autorelease();  
		va_end(args);  
		return pRet;  
	}  
	va_end(args);  
	CC_SAFE_DELETE(pRet);
	return NULL; 
}