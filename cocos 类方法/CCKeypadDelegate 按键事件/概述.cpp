2014-02-24
	布景层类CCLayer和其子类继承自CCKeypadDelegate类，因此
布景层类和其子类可以使用获得加速度传感器的方法。
	要在CCLay中获得按键事件的数据，首先要在初始化函数中加入
允许接收按键事件的函数。
	eg：
	KeypadTest::KeypadTest()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* label = CCLabelTTF::create("Keypad Test", "Arial", 28);
    addChild(label, 0);
    label->setPosition( ccp(s.width/2, s.height-50) );

	/// 设置允许接收按键事件
    setKeypadEnabled(true);

    // create a label to display the tip string
    m_pLabel = CCLabelTTF::create("Please press any key...", "Arial", 22);
    m_pLabel->setPosition(ccp(s.width / 2, s.height / 2));
    addChild(m_pLabel, 0);
    
    m_pLabel->retain();
}
/// 重写返回键事件
void KeypadTest::keyBackClicked()
{
    m_pLabel->setString("BACK clicked!");
}
/// 重写Menu事件
/// 重写Menu事件
void KeypadTest::keyMenuClicked()
{
    m_pLabel->setString("MENU clicked!");
}