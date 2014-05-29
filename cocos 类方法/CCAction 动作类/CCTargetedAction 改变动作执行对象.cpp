2014-02-19
	一般默认动作的执行对象时调用runAction的对象；如果需要自定义动作的执行对象，需要调用
CCTargetedAction。
void ActionTargeted::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(2);


    CCJumpBy* jump1 = CCJumpBy::create(2,CCPointZero,100,3);
    CCJumpBy* jump2 = (CCJumpBy*)jump1->copy()->autorelease();
    CCRotateBy* rot1 =  CCRotateBy::create(1, 360);
    CCRotateBy* rot2 = (CCRotateBy*)rot1->copy()->autorelease();

	/// 执行动作的节点  须执行的动作
    CCTargetedAction *t1 = CCTargetedAction::create(m_kathia, jump2);
    CCTargetedAction *t2 = CCTargetedAction::create(m_kathia, rot2);


    CCSequence* seq = (CCSequence*)CCSequence::create(jump1, t1, rot1, t2, NULL);
    CCRepeatForever *always = CCRepeatForever::create(seq);
	/// 执行到t1和t2时，动作节点变为m_kathia
    m_tamara->runAction(always);
}