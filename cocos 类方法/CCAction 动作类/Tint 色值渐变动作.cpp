色值变化
2014-2-18
void ActionTint::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);
	/// 色调变化到设定值 时间间隔 R G B
    CCActionInterval*  action1 = CCTintTo::create(2, 255, 0, 255);
	/// 色调变化相对数值 时间间隔 R G B
    CCActionInterval*  action2 = CCTintBy::create(2, -127, -255, -127);
    CCActionInterval*  action2Back = action2->reverse();

    m_tamara->runAction( action1);
    m_kathia->runAction( CCSequence::create( action2, action2Back, NULL));
}