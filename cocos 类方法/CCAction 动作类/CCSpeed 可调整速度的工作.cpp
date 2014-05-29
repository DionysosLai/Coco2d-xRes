2014-02-19
	 不是一个独立的动作，可理解是对目前动作的一个“包装”，经过这个“包装”后，实现
运动的“慢动作”和“快进”效果。
void SpeedTest::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    // rotate and jump
    CCActionInterval *jump1 = CCJumpBy::create(4, ccp(-s.width+80, 0), 100, 4);
    CCActionInterval *jump2 = jump1->reverse();
    CCActionInterval *rot1 = CCRotateBy::create(4, 360*2);
    CCActionInterval *rot2 = rot1->reverse();
    
    CCSequence* seq3_1 = CCSequence::create(jump2, jump1, NULL);
    CCSequence* seq3_2 = CCSequence::create( rot1, rot2, NULL);
    CCSpawn* spawn = CCSpawn::create(seq3_1, seq3_2, NULL);
    CCSpeed* action = CCSpeed::create(CCRepeatForever::create(spawn), 1.0f);
    action->setTag(kTagAction1);
    
    CCAction* action2 = (CCAction*)(action->copy()->autorelease());
    CCAction* action3 = (CCAction*)(action->copy()->autorelease());

    action2->setTag(kTagAction1);
    action3->setTag(kTagAction1);
    
    m_grossini->runAction(action2);
    m_tamara->runAction(action3);
    m_kathia->runAction(action);
    
	/// 每1s调用SpeedTest::altertime函数
    this->schedule(schedule_selector(SpeedTest::altertime), 1.0f);//:@selector(altertime:) interval:1.0f];
}

void SpeedTest::altertime(float dt)
{    
    CCSpeed* action1 = (CCSpeed*)(m_grossini->getActionByTag(kTagAction1));
    CCSpeed* action2 = (CCSpeed*)(m_tamara->getActionByTag(kTagAction1));
    CCSpeed* action3 = (CCSpeed*)(m_kathia->getActionByTag(kTagAction1));
    
	///　1：原速度  小于1：减速 大于1：加速；
    action1->setSpeed( CCRANDOM_MINUS1_1() * 2 );
    action2->setSpeed( CCRANDOM_MINUS1_1() * 2 );
    action3->setSpeed( CCRANDOM_MINUS1_1() * 2 );
}