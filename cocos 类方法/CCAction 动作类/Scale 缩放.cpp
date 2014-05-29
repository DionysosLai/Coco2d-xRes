缩放
2014-2-18
void ActionScale::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);
	/// 缩放到相应的比列 参数1是时间间隔，参数2是缩放比列
    CCActionInterval*  actionTo = CCScaleTo::create(2.0f, 0.5f);
	/// 在目前基础上缩放相应比例参数1是时间间隔，参数2是x缩放比列 参数3是y缩放比列
    CCActionInterval*  actionBy = CCScaleBy::create(2.0f, 1.0f, 10.0f);
    CCActionInterval*  actionBy2 = CCScaleBy::create(2.0f, 5.0f, 1.0f);

    m_grossini->runAction( actionTo);
    m_tamara->runAction( CCSequence::create(actionBy, actionBy->reverse(), NULL));
    m_kathia->runAction( CCSequence::create(actionBy2, actionBy2->reverse(), NULL));
}