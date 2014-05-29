void BackScreen::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void BackScreen::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() >= 2)	///< 至少2点
	{
		CCSetIterator iter	= pTouches->begin();
		CCPoint mPoint1		= ((CCTouch*)(*iter))->getLocationInView();
		mPoint1				= CCDirector::sharedDirector()->convertToGL(mPoint1);

		iter++;
		CCPoint mPoint2		= ((CCTouch*)(*iter))->getLocationInView();
		mPoint2				= CCDirector::sharedDirector()->convertToGL(mPoint2);

		m_lDistance			= sqrt((mPoint1.x - mPoint2.x)*(mPoint1.x - mPoint2.x) +
								(mPoint1.y - mPoint2.y)*(mPoint1.y - mPoint2.y));
		m_iDeltax			= (mPoint1.x + mPoint2.x)/2 - m_pSpriteHero->getPositionX();
		m_iDeltay			= (mPoint1.y + mPoint2.y)/2 - m_pSpriteHero->getPositionY();
	}
}

void BackScreen::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (pTouches->count() >= 2)
	{
		CCSetIterator iter	= pTouches->begin();
		CCPoint mPoint1		= ((CCTouch*)(*iter))->getLocationInView();
		mPoint1				= CCDirector::sharedDirector()->convertToGL(mPoint1);

		iter++;
		CCPoint mPoint2		= ((CCTouch*)(*iter))->getLocationInView();
		mPoint2				= CCDirector::sharedDirector()->convertToGL(mPoint2);

		long mDistance		= sqrt((mPoint1.x - mPoint2.x)*(mPoint1.x - mPoint2.x) +
			(mPoint1.y - mPoint2.y)*(mPoint1.y - mPoint2.y));

		m_fScale			= mDistance / m_lDistance * m_fScale;	///< 改变缩放值；
		m_lDistance			= mDistance;
		m_pSpriteHero->setScale(m_fScale);
		
		float x				= (mPoint1.x + mPoint2.x)/2 - m_iDeltax;
		float y				= (mPoint1.y + mPoint2.y)/2 - m_iDeltay;
		m_pSpriteHero->setPosition(ccp(x, y));
		m_iDeltax			= (mPoint1.x + mPoint2.x)/2 - m_pSpriteHero->getPositionX();
		m_iDeltay			= (mPoint1.y + mPoint2.y)/2 - m_pSpriteHero->getPositionY();
	}
}

void BackScreen::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}

void BackScreen::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}