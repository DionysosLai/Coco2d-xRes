需委托触摸协议：
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
		
同时响应ccTouchEnd时，必须先响应ccTouchBegan();