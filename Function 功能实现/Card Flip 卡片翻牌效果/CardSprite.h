#ifndef __CARD_SPRITE_H__
#define __CARD_SPRITE_H__

#include "cocos2d.h"  

USING_NS_CC;  

class CardSprite : public CCSprite  
{  
public:  
	CardSprite();  
	~CardSprite();  

	CCSize getContentSize();
	
	static CardSprite* create(const char* inCardImageName, const char* outCardImageName, float duration);  
	virtual bool init(const char* inCardImageName, const char* outCardImageName, float duration);  

private:  
	bool m_isOpened;  
	CCActionInterval* m_openAnimIn;  
	CCActionInterval* m_openAnimOut;
	CCActionInterval* m_slideDown;
	CCActionInterval* m_slideUp;

	void initData(const char* inCardImageName, const char* outCardImageName, float duration);  

public:  
	void openCard(); 
	void closeCard(void);
	void slideDown(void);
	void slideUp(void);
};  

#endif /* defined(__CARD_SPRITE_H__) */  