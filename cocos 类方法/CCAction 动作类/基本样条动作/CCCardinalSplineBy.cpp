基本样条动作
2014-2-18
void ActionCardinalSpline::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(2);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCPointArray *array = CCPointArray::create(20);
    
    array->addControlPoint(ccp(0, 0));
    array->addControlPoint(ccp(s.width/2-30, 0));
    array->addControlPoint(ccp(s.width/2-30, s.height-80));
    array->addControlPoint(ccp(0, s.height-80));
    array->addControlPoint(ccp(0, 0));
    
    //
    // sprite 1 (By)
    //
    // Spline with no tension (tension==0)
    //
    ///相对（设置点数组时，第一个点要设置为（0,0）） 时间间隔 点数组 拉力系数
    CCCardinalSplineBy *action = CCCardinalSplineBy::create(3, array, 0);
    CCActionInterval *reverse = action->reverse();
    
    CCFiniteTimeAction *seq = CCSequence::create(action, reverse, NULL);
    
    m_tamara->setPosition(ccp(50, 50));
    m_tamara->runAction(seq);
    
    //
    // sprite 2 (By)
    //
    // Spline with high tension (tension==1)
    //
    
    CCCardinalSplineBy *action2 = CCCardinalSplineBy::create(3, array, 1);
    CCActionInterval *reverse2 = action2->reverse();
    
    CCFiniteTimeAction *seq2 = CCSequence::create(action2, reverse2, NULL);
    
    m_kathia->setPosition(ccp(s.width/2, 50));
    m_kathia->runAction(seq2);
    
    m_pArray = array;
    array->retain();
}