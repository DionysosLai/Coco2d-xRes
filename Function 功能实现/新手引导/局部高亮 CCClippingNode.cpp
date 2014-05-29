	//创建一个裁剪节点用来实现遮罩的效果
	CCClippingNode * clippingNode = CCClippingNode::create();
	this->addChild(clippingNode,10);	/// 在最前方
	//向裁剪节点中加入内容，这里加入的是一个透明的层---即位底板
	CCLayerColor * layer = CCLayerColor::create(ccc4(0,0,0,255));
	clippingNode->addChild(layer);
	//创建模板，裁剪节点将按照这个模板来裁剪区域---即位模板
	m_spNode = CCSprite::create("PIC_UI_JT_XX.png");
	m_spNode->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
	clippingNode->setStencil(m_spNode);
	//这个是用来设置显示裁剪区域还是非裁剪区域的
	clippingNode->setInverted(true);
	//我们之前放了一张裁剪的模板，按照这个模板裁剪的时候同时按照这个alpha的值裁剪，这个值的范围是0-1
	//设为0就把透明的区域裁剪掉了
	clippingNode->setAlphaThreshold(0);
	
	
	显示的部分是”底板-模板“，也就是模板处会高亮；
	
	
getStencil：返回一个节点对象，这个对象就是之前提到的“裁减模板”。
setStencil：设置“裁减模板”。
getAlphaThreshold:：这种裁减是可以改变裁减的透明度的，修改这个透明度就是通过设置这个阈值。
setAlphaThreshold：获得这个透明度阈值。
isInverted：之前说过的剪刀剪形状的例子，剪完形状以后，是显示被剪掉的部分，还是显示剩余的部分呢，默认isInverted值是false，是显示被剪掉的部分，设置为true则是显示剩余的部分。这个函数获得这个值。
setInverted：设置isInverted值。