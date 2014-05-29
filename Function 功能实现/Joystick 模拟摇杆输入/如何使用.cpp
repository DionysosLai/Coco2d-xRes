	CCSprite *testPointL=CCSprite::create("point.png");//摇杆  
	CCSprite *testBGL=CCSprite::create("joytickbg.png");//摇杆背景  
	Joystick *testJSL=Joystick::JoystickWithCenter(ccp(visibleSize.width/2,visibleSize.height/2), testPointL ,testBGL);  
	this->addChild(testJSL, 10);  
	testJSL->Active();  