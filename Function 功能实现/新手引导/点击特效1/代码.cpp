	//创建cliper
	CCClippingNode* pClip=CCClippingNode::create();
	/// 设置显示裁剪部分
	pClip->setInverted(true);
	addChild(pClip);
	//添加底板
	CCLayerColor* pColor=CCLayerColor::create(ccc4(0,0,0,200));
	pClip->addChild(pColor);

	//绘制圆形区域
	static ccColor4F green = {0, 1, 0, 1};//顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色
	float fRadius=55.0f;//圆的半径
	const int nCount=100;//圆形其实可以看做正多边形,我们这里用正100边型来模拟园
	const float coef = 2.0f * (float)M_PI/nCount;//计算每两个相邻顶点与中心的夹角
	static CCPoint circle[nCount];//顶点数组
	for(unsigned int i = 0;i <nCount; i++) {
		float rads = i*coef;//弧度
		circle[i].x = fRadius * cosf(rads);//对应顶点的x
		circle[i].y = fRadius * sinf(rads);//对应顶点的y
	}
	CCDrawNode *pStencil=CCDrawNode::create();
	pStencil->drawPolygon(circle, nCount, green, 0, green);//绘制这个多边形!

	//动起来
	pStencil->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCScaleBy::create(0.05f, 0.95f),
		CCScaleTo::create(0.125f, 1))));
	pStencil->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	//设这模板
	pClip->setStencil(pStencil);

	//添加圆和手的图片
	CCSprite*pCircle=CCSprite::create("circle.png");
	pCircle->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pCircle->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCScaleBy::create(0.05f, 0.95f),
		CCScaleTo::create(0.125f, 1))));
	addChild(pCircle);

	CCSprite* pHand=CCSprite::create("hand.png");
	pHand->setAnchorPoint(ccp(0.0f,1.0f));
	pHand->setPosition(ccp(pCircle->getContentSize().width/2,pCircle->getContentSize().height/2));
	pCircle->addChild(pHand);