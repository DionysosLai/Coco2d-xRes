2014-02-19
void ActionCatmullRomStacked::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(2);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    //
    // sprite 1 (By)
    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Catmull Rom curve, it is better to start with (0,0).
    //
    
    m_tamara->setPosition(ccp(50,50));
    
    CCPointArray *array = CCPointArray::create(20);
    
    array->addControlPoint(ccp(0,0));
    array->addControlPoint(ccp(80,80));
    array->addControlPoint(ccp(s.width-80,80));
    array->addControlPoint(ccp(s.width-80,s.height-80));
    array->addControlPoint(ccp(80,s.height-80));
    array->addControlPoint(ccp(80,80));
    array->addControlPoint(ccp(s.width/2, s.height/2));
    /// 跟CCCardinalSplineBy类似，少了个拉力系数
    CCCatmullRomBy *action = CCCatmullRomBy::create(3, array);
    CCCatmullRomBy* reverse = (CCCatmullRomBy*)action->reverse();
    
    CCSequence *seq = CCSequence::create(action, reverse, NULL);
    
    m_tamara->runAction(seq);
    
    
    m_tamara->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCMoveBy::create(0.05f, ccp(10,0)),
       CCMoveBy::create(0.05f, ccp(-10,0)),
       NULL)));
    
    
    //
    // sprite 2 (To)
    //
    // The startPosition is not important here, because it uses a "To" action.
    // The initial position will be the 1st point of the Catmull Rom path
    //
    
    CCPointArray *array2 = CCPointArray::create(20);
    
    array2->addControlPoint(ccp(s.width/2, 30));
    array2->addControlPoint(ccp(s.width-80,30));
    array2->addControlPoint(ccp(s.width-80,s.height-80));
    array2->addControlPoint(ccp(s.width/2,s.height-80));
    array2->addControlPoint(ccp(s.width/2, 30));
    
    
    CCCatmullRomTo *action2 = CCCatmullRomTo::create(3, array2);
    CCCatmullRomTo* reverse2 = (CCCatmullRomTo*)action2->reverse();
    
    CCSequence *seq2 = CCSequence::create(action2, reverse2, NULL);
    
    m_kathia->runAction(seq2);
    
    
    m_kathia->runAction(
     CCRepeatForever::create(
      CCSequence::create(
       CCMoveBy::create(0.05f, ccp(10,0)),
       CCMoveBy::create(0.05f, ccp(-10,0)),
       NULL)));
    
    
    array->retain();
    _array1 = array;
    array2->retain();
    _array2 = array2;
}