#include "CardSprite.h"  

#define kInAngleZ        270 ///< 里面卡牌的起始Z轴角度  
#define kInDeltaZ        90  ///< 里面卡牌旋转的Z轴角度差  
#define kOutAngleZ       0   ///< 封面卡牌的起始Z轴角度  
#define kOutDeltaZ       90  ///< 封面卡牌旋转的Z轴角度差  

#define kDownAngleZ     0	///< 向下卡牌的起始Z轴角度  
#define kDownDeltaZ     90  ///< 向下卡牌旋转的Z轴角度差  
#define kUpAngleZ       270 ///< 向上卡牌的起始Z轴角度  
#define kUpDeltaZ       90  ///< 向上卡牌旋转的Z轴角度差  

#define kSlideAngleX	90 ///< 向上卡牌的起始Z轴角度  
#define kSlipeDeltaX	0  ///< 向上卡牌旋转的Z轴角度差  

enum {  
	tag_inCard = 1,  
	tag_outCard  
};  

CardSprite::CardSprite()  
{  

}  

CardSprite::~CardSprite()  
{  
	m_openAnimIn->release();  
	m_openAnimOut->release();  
}  

CardSprite* CardSprite::create(const char* inCardImageName, const char* outCardImageName, float duration)  
{  
	CardSprite *pSprite = new CardSprite();  
	if (pSprite && pSprite->init(inCardImageName, outCardImageName, duration))  
	{  
		pSprite->autorelease();  
		return pSprite;  
	}  
	CC_SAFE_DELETE(pSprite);  
	return NULL;  
}  

bool CardSprite::init(const char* inCardImageName, const char* outCardImageName, float duration)  
{  
	if (!CCSprite::init())  
	{  
		return false;  
	}  
	initData(inCardImageName, outCardImageName, duration);  
	return true;  
}  
 
void CardSprite::initData(const char* inCardImageName, const char* outCardImageName, float duration)  
{  
	m_isOpened = false;  

	CCSprite* inCard = CCSprite::create(inCardImageName);  
	inCard->setPosition(CCPointZero);  
	inCard->setVisible(false);  
	inCard->setTag(tag_inCard);  
	addChild(inCard);  

	CCSprite* outCard = CCSprite::create(outCardImageName);  
	outCard->setPosition(CCPointZero);  
	outCard->setTag(tag_outCard);  
	addChild(outCard);  

	m_openAnimIn = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration * .5),  
		CCShow::create(),  
		CCOrbitCamera::create(duration * .5, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),  
		NULL);  
	m_openAnimIn->retain();  

	m_openAnimOut = (CCActionInterval *)CCSequence::create(CCOrbitCamera::create(duration * .5, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),  
		CCHide::create(),  
		CCDelayTime::create(duration * .5),  
		NULL);  
	m_openAnimOut->retain();

	m_slideUp = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration * .5),  
		CCShow::create(),  
		CCOrbitCamera::create(duration * .5, 1, 0, kUpAngleZ, kUpDeltaZ, kSlideAngleX, kSlipeDeltaX),  
		NULL);  
	m_slideUp->retain();
	
	m_slideDown = (CCActionInterval *)CCSequence::create(CCOrbitCamera::create(duration * .5, 1, 0, kDownAngleZ, kDownDeltaZ, kSlideAngleX, kSlipeDeltaX),  
		CCHide::create(),  
		CCDelayTime::create(duration * .5),  
		NULL);  
	m_slideDown->retain();


  
}  
 
void CardSprite::openCard()  
{    
	CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard); 
	CCSprite* outCard = (CCSprite*)getChildByTag(tag_outCard); 
	/// 没有stop的话，再次openCard会出错；
	inCard->stopAllActions() ;
	outCard->stopAllActions() ; 

	inCard->runAction(m_openAnimIn); 
	outCard->runAction(m_openAnimOut); 
}

CCSize CardSprite:: getContentSize() 
{ 
	CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard); 
	return inCard->getContentSize();
}

void CardSprite::closeCard( void )
{
	CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard); 
	CCSprite* outCard = (CCSprite*)getChildByTag(tag_outCard); 
	/// 没有stop的话，再次openCard会出错；
	inCard->stopAllActions() ;
	outCard->stopAllActions() ; 

	inCard->runAction(m_openAnimOut); 
	outCard->runAction(m_openAnimIn); 
}

void CardSprite::slideDown( void )
{
	CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard); 
	CCSprite* outCard = (CCSprite*)getChildByTag(tag_outCard); 
	/// 没有stop的话，再次openCard会出错；
	inCard->stopAllActions() ;
	outCard->stopAllActions() ; 

	inCard->runAction(m_slideUp); 
	outCard->runAction(m_slideDown); 
}

void CardSprite::slideUp( void )
{
	CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard); 
	CCSprite* outCard = (CCSprite*)getChildByTag(tag_outCard); 
	/// 没有stop的话，再次openCard会出错；
	inCard->stopAllActions() ;
	outCard->stopAllActions() ; 

	inCard->runAction(m_slideDown); 
	outCard->runAction(m_slideUp); 
}
