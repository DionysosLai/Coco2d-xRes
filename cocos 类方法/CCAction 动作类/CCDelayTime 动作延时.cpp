2014-02-19
	使动作延后一段时间执行；理解为“空动作”，只有时间，没有任何动作；
	void ActionDelayTime::onEnter()
	{
		ActionsDemo::onEnter();

		alignSpritesLeft(1);

		/// 执行完第一个参数后，不操作2s，在执行第三个参数；
		CCActionInterval*  move = CCMoveBy::create(1, ccp(150,0));
		CCFiniteTimeAction*  action = CCSequence::create( move, CCDelayTime::create(2), move, NULL);

		m_grossini->runAction(action);
	}
