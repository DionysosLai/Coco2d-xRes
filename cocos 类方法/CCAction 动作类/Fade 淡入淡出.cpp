渐入渐出动作
2014-2-18
void ActionFade::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);	；

    m_tamara->setOpacity( 0 );///< 淡入FadeIn要设置不透明度为0
	/// 参数为时间间隔
    CCActionInterval*  action1 = CCFadeIn::create(1.0f);
    CCActionInterval*  action1Back = action1->reverse();

    CCActionInterval*  action2 = CCFadeOut::create(1.0f);
    CCActionInterval*  action2Back = action2->reverse();

    m_tamara->runAction( CCSequence::create( action1, action1Back, NULL));
    m_kathia->runAction( CCSequence::create( action2, action2Back, NULL));
}