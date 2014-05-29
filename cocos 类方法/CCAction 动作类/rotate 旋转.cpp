旋转
2014-2-18
void ActionRotate::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);
	/// 移动到相应的角度 参数1是时间间隔，参数2是角度
    CCActionInterval*  actionTo = CCRotateTo::create( 2, 45);
    CCActionInterval*  actionTo2 = CCRotateTo::create( 2, -45);
    CCActionInterval*  actionTo0 = CCRotateTo::create(2 , 0);
    m_tamara->runAction( CCSequence::create(actionTo, actionTo0, NULL));
	/// 移动相应的角度，是在目前的角度上加上相对值 参数1是时间间隔，参数2是角度
    CCActionInterval*  actionBy = CCRotateBy::create(2 ,  360);
    CCActionInterval*  actionByBack = actionBy->reverse();
    m_grossini->runAction( CCSequence::create(actionBy, actionByBack, NULL));

    m_kathia->runAction( CCSequence::create(actionTo2, actionTo0->copy()->autorelease(), NULL));
}