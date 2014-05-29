扭曲
2014-2-18
void ActionSkew::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);
	/// 扭曲效果到设定的参数 时间间隔 x轴扭曲参数 y轴扭曲参数
    CCActionInterval *actionTo = CCSkewTo::create(2, 37.2f, -37.2f);
    CCActionInterval *actionToBack = CCSkewTo::create(2, 0, 0);
	/// 扭曲相对参数
    CCActionInterval *actionBy = CCSkewBy::create(2, 0.0f, -90.0f);
    CCActionInterval *actionBy2 = CCSkewBy::create(2, 45.0f, 45.0f);
    CCActionInterval *actionByBack = actionBy->reverse();

    m_tamara->runAction(CCSequence::create(actionTo, actionToBack, NULL));
    m_grossini->runAction(CCSequence::create(actionBy, actionByBack, NULL));

    m_kathia->runAction(CCSequence::create(actionBy2, actionBy2->reverse(), NULL));
}