	CCSprite* sprite1 = CCSprite::create("png1.png");		///< 普通
	sprite1->setPosition(ccp(visibleSize.width/4.f, visibleSize.height/2.f));
	this->addChild(sprite1, 1);

	EXGraySprite* sprite2 = EXGraySprite::create("png1.png");	///< 灰色图片
	sprite2->setPosition(ccp(visibleSize.width/4.f * 3.f, visibleSize.height/2.f));
	this->addChild(sprite2, 1);