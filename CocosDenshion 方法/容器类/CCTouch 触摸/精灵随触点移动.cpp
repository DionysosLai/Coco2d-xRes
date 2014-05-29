精灵随触点移动
2014-02-18
void BackScreen::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	/// 获取触点指针容器中的第一个元素
	CCSetIterator it = pTouches->begin();
	/// 将其转化为触点信息
	CCTouch* touch = (CCTouch*)(*it);
	/// 取得触点位置
	CCPoint touchLocation = touch->getLocation();
	/// 设置精灵在触点位置
	m_pSprite->setPosition(touchLocation);
}