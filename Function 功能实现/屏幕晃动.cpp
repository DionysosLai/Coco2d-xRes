///@brief 屏幕晃动
///@return 
	void screenShake();

void GameScene::screenShake()
{
	int X = 10, Y = 10;
	CCActionInterval* move1		=  CCMoveBy::create(0.05f,ccp( CCRANDOM_0_1()*X-X/2,CCRANDOM_0_1()*Y-Y/2));
	CCActionInterval* move1r	= move1->reverse();
	CCActionInterval* move2		=  CCMoveBy::create(0.05f,ccp( CCRANDOM_0_1()*X-X/2,CCRANDOM_0_1()*Y-Y/2));
	CCActionInterval* move2r	= move2->reverse();
	CCActionInterval* move3		=  CCMoveBy::create(0.05f,ccp( CCRANDOM_0_1()*X-X/2,CCRANDOM_0_1()*Y-Y/2));
	CCActionInterval* move3r	= move3->reverse();
	CCActionInterval* move4		=  CCMoveBy::create(0.05f,ccp( CCRANDOM_0_1()*X-X/2,CCRANDOM_0_1()*Y-Y/2));
	CCActionInterval* move4r	= move4->reverse();
	CCActionInterval* move5		=  CCMoveBy::create(0.05f,ccp( CCRANDOM_0_1()*X-X/2,CCRANDOM_0_1()*Y-Y/2));
	CCActionInterval* move5r	= move5->reverse();
	CCActionInterval* move6		=  CCMoveBy::create(0.05f,ccp( CCRANDOM_0_1()*X-X/2,CCRANDOM_0_1()*Y-Y/2));
	CCActionInterval* move6r	= move6->reverse();
	CCFiniteTimeAction* action1 = CCSequence::create(move1,move1r,move2,move2r,move3,move3r,move4,move4r,move5,move5r,move6,move6r,NULL);
	this->stopAllActions();
	this->runAction(action1);
}	