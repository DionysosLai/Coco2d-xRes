2014-02-24
	布景层类CCLayer和其子类继承自CCAccelerometerDelegate类，因此
布景层类和其子类可以使用获得加速度传感器的方法。
	例子：
void AccelerometerTest::onEnter()
{
    CCLayer::onEnter();

    setAccelerometerEnabled(true);


    CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 32);
    addChild(label, 1);
    label->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y-50) );

    m_pBall = CCSprite::create("Images/ball.png");
    m_pBall->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y));
    addChild(m_pBall);

    m_pBall->retain();
}
/// 重写didAccelerate函数
void AccelerometerTest::didAccelerate(CCAcceleration* pAccelerationValue)
{
//     double fNow = pAccelerationValue->timestamp;
// 
//     if (m_fLastTime > 0.0)
//     {
//         CCPoint ptNow = convertToUI
//     }
// 
//     m_fLastTime = fNow;

    CCDirector* pDir = CCDirector::sharedDirector();

    /*FIXME: Testing on the Nexus S sometimes m_pBall is NULL */
    if ( m_pBall == NULL ) {
        return;
    }

    CCSize ballSize  = m_pBall->getContentSize();

    CCPoint ptNow  = m_pBall->getPosition();
    CCPoint ptTemp = pDir->convertToUI(ptNow);

    ptTemp.x += pAccelerationValue->x * 9.81f;
    ptTemp.y -= pAccelerationValue->y * 9.81f;

    CCPoint ptNext = pDir->convertToGL(ptTemp);
    FIX_POS(ptNext.x, (VisibleRect::left().x+ballSize.width / 2.0), (VisibleRect::right().x - ballSize.width / 2.0));
    FIX_POS(ptNext.y, (VisibleRect::bottom().y+ballSize.height / 2.0), (VisibleRect::top().y - ballSize.height / 2.0));
    m_pBall->setPosition(ptNext);
}