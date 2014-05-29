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