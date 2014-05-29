闪烁动作
2014-2-18
void ActionBlink::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);
	///　时间间隔 闪烁次数
    CCActionInterval*  action1 = CCBlink::create(2, 10);
    CCActionInterval*  action2 = CCBlink::create(2, 5);

    m_tamara->runAction( action1);
    m_kathia->runAction(action2);
}