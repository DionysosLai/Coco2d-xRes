贝塞尔曲线动作
2014-2-18
void ActionBezier::onEnter()
{
    ActionsDemo::onEnter();

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Bezier curve, make it (0,0)
    //

    centerSprites(3);

    // sprite 1
    ccBezierConfig bezier;
    bezier.controlPoint_1 = ccp(0, s.height/2);
    bezier.controlPoint_2 = ccp(300, -s.height/2);
    bezier.endPosition = ccp(300,100);
	/// 时间间隔，参数2中贝塞尔的终点位置是相对位置
    CCActionInterval*  bezierForward = CCBezierBy::create(3, bezier);
    CCActionInterval*  bezierBack = bezierForward->reverse();    
    CCAction*  rep = CCRepeatForever::create(CCSequence::create( bezierForward, bezierBack, NULL));


    // sprite 2
    m_tamara->setPosition(ccp(80,160));
    ccBezierConfig bezier2;
    bezier2.controlPoint_1 = ccp(100, s.height/2);
    bezier2.controlPoint_2 = ccp(200, -s.height/2);
    bezier2.endPosition = ccp(240,160);
	/// 时间间隔，参数2中贝塞尔的终点位置是绝对位置
    CCActionInterval*  bezierTo1 = CCBezierTo::create(2, bezier2);    

    // sprite 3
    m_kathia->setPosition(ccp(400,160));
    CCActionInterval*  bezierTo2 = CCBezierTo::create(2, bezier2);

    m_grossini->runAction( rep);
    m_tamara->runAction(bezierTo1);
    m_kathia->runAction(bezierTo2);

}