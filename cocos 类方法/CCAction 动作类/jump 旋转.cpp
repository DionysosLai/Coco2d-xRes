跳跃
2014-2-18
void ActionJump::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);
	/// 跳跃到某个固定的位置 时间间隔 目标位置绝对坐标 跳跃高度，跳跃次数
    CCActionInterval*  actionTo = CCJumpTo::create(2, ccp(300,300), 50, 4);
	/// 按抛物线轨迹移动相应距离，参数是相对的距离。时间间隔 目标位置相对坐标 跳跃高度，跳跃次数
    CCActionInterval*  actionBy = CCJumpBy::create(2, ccp(300,0), 50, 4);
    CCActionInterval*  actionUp = CCJumpBy::create(2, ccp(0,0), 80, 4);
    CCActionInterval*  actionByBack = actionBy->reverse();

    m_tamara->runAction( actionTo);
    m_grossini->runAction( CCSequence::create(actionBy, actionByBack, NULL));
    m_kathia->runAction( CCRepeatForever::create(actionUp));
}