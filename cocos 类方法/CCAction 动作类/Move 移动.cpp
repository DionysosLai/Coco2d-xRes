移动
2014-2-18
void ActionMoveonEnter()
{
    ActionsDemoonEnter();

    centerSprites(3);

    CCSize s = CCDirectorsharedDirector()-getWinSize();
	/// 到目标位置 参数1为时间，参数2为位置对象
    CCActionInterval*  actionTo = CCMoveTo::create(2, ccp(s.width-40, s.height-40)); 
	/// 从当前位置到目标位置；参数1为时间，参数2为位置对象
    CCActionInterval*  actionBy = CCMove::Bycreate(2, ccp(80,80));
    CCActionInterval*  actionByBack = actionBy-reverse();

    m_tamara-runAction( actionTo);
    m_grossini-runAction( CCSequencecreate(actionBy, actionByBack, NULL));
    m_kathia-runAction(CCMoveTocreate(1, ccp(40,40)));
}
