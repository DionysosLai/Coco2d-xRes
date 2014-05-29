2014-02-19
CCFollow 一个动作跟随另一个动作运动；
void ActionFollow::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(1);
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    m_grossini->setPosition(ccp(-200, s.height / 2));
    CCActionInterval* move      = CCMoveBy::create(2, ccp(s.width * 3, 0));
    CCActionInterval* move_back = move->reverse();
    CCSequence* seq       = CCSequence::create(move, move_back, NULL);
    CCAction* rep               = CCRepeatForever::create(seq);

    m_grossini->runAction(rep);

	/// 参数1：要跟随的节点 参数2：运动边界，若没有存入边界，视为没有边界
    this->runAction(CCFollow::create(m_grossini, CCRectMake(0, 0, s.width * 2 - 100, s.height)));
}