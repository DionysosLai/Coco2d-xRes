2014-02-19
	有时候某些动作完成后，需要执行一些数据上的处理，比如攻击一个敌人，
之后需要加减雪之类，这时就需要调用回调动作CCCallFunc。有子类CCCallFuncN
和CCCallFunctionO，其中CCCallFuncN有子类CCCallFuncND。
	CCCallFunc的回调函数以CCNode的对象和数据作为参数，CCCallFuncO以CCObject
作为回调函数的参数。

void ActionSequence2::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    m_grossini->setVisible(false);

    CCFiniteTimeAction*  action = CCSequence::create(
        CCPlace::create(ccp(200,200)),
        CCShow::create(),
        CCMoveBy::create(1, ccp(100,0)),
		/// 回调函数的对象  参数2：通过相应选择器获得函数名称；
        CCCallFunc::create(this, callfunc_selector(ActionSequence2::callback1)),
        CCCallFuncN::create(this, callfuncN_selector(ActionSequence2::callback2)),
		/// 最后一个参数是数据对象；
        CCCallFuncND::create(this, callfuncND_selector(ActionSequence2::callback3), (void*)0xbebabeba),
        NULL);

    m_grossini->runAction(action);
}

void ActionSequence2::callback1()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *label = CCLabelTTF::create("callback 1 called", "Marker Felt", 16);
    label->setPosition(ccp( s.width/4*1,s.height/2));

    addChild(label);
}

void ActionSequence2::callback2(CCNode* sender)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *label = CCLabelTTF::create("callback 2 called", "Marker Felt", 16);
    label->setPosition(ccp( s.width/4*2,s.height/2));

    addChild(label);
}

void ActionSequence2::callback3(CCNode* sender, void* data)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *label = CCLabelTTF::create("callback 3 called", "Marker Felt", 16);
    label->setPosition(ccp( s.width/4*3,s.height/2));

    addChild(label);
}