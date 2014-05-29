2014-03-21
初始化：
CCSprite* spriteTime = CCSprite::create("joytickbg.png");	///< 是一张灰色的图片
pt = CCProgressTimer::create(spriteTime);
pt->setScale(0.5f);
pt->setPosition(visibleSize.width/2.0f, visibleSize.height/2.0f);
pt->setReverseProgress(true);		///< ture-设置顺时针扫描， false-设置逆时针扫描
m_percent	= 100.0f;
pt->setPercentage(m_percent);

然后在void update(float delta)
{
	m_percent--;
	if (0 > m_percent)
	{
		m_percent = 100.0f;
	}
	pt->setPercentage(m_percent);
}