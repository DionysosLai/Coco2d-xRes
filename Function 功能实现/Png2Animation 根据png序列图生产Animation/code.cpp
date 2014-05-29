	CCTexture2D *playerRunTexture = CCTextureCache::sharedTextureCache()->addImage("runman.png");    
	CCArray* animFrames = CCArray::createWithCapacity(14);    
	for(int i=0;i<8;i++){    
		CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(playerRunTexture, cocos2d::CCRectMake(72*i, 0, 72, 72));    
		animFrames->addObject(frame);    
	}    
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);    
	CCSprite* mSprite = CCSprite::create();    
	mSprite->setPosition(ccp(0 , visibleSize.height/2));    
	mSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));    
	this->addChild(mSprite, 10, 10);  