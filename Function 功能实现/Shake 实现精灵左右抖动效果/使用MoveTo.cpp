2014-03-24
	CCSprite* ball = CCSprite::create("point.png");
	ball->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	CCPoint pointL = ball->getPosition();
	CCPoint pointR = ball->getPosition();
	pointL.x -= 3;
	pointR.x += 3;

	CCMoveTo* moveToL = CCMoveTo::create(0.08f, pointL);
	CCMoveTo* moveToR = CCMoveTo::create(0.08f, pointR);
	CCFiniteTimeAction* action = CCSequence::create(moveToL, moveToR, NULL);
	CCActionInterval* shake = CCRepeatForever::create((CCActionInterval*)action);
	ball->stopAllActions();
	ball->runAction(shake);
	addChild(ball,10);