2014-02-24 关键代码；
重写代码：
void BackScreen::registerWithTouchDispatcher()
{
	CCDirector* pDiretor = CCDirector::sharedDirector();
	pDiretor->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool BackScreen::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint posHero		= m_pSpriteHero->getPosition();
	CCPoint location	= pTouch->getLocationInView();
	location			= CCDirector::sharedDirector()->convertToGL(location);

	
	if (location.x - posHero.x > -22.5 && location.x - posHero.x < 22.5 &&
		location.y - posHero.y > -22.5 && location.y - posHero.y < 22.5)
	{
		m_bControl	= true;
		///偏移，为了不使主角会移动到手指中央；
		m_iDeltax	= location.x - posHero.x;
		m_iDeltay	= location.y - posHero.y;
	}
	return true;
}


void BackScreen::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_bControl)
	{
		CCPoint location = pTouch->getLocationInView();
		location		 = CCDirector::sharedDirector()->convertToGL(location);

		float x = location.x - m_iDeltax;
		float y = location.y - m_iDeltay;

		m_pSpriteHero->setPosition(ccp(x, y));
	}
}

void BackScreen::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	m_bControl = false;
}

void BackScreen::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	m_bControl = false;
}
