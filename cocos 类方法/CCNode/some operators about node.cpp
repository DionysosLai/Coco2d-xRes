/*
* 首先通过create函数创建一个节点anode，把anode作为子节点使用addChild函数加入
* BackScreen场景类对象中；然后把本来作为子节点加
* 入BackScreen场景类对象中的对象，使用addChild
* 函数作为子节点加入anode中。这些对象包括菜单类
* 对象、标签类对象、人物精灵类对象。这些对象的类
* 都是CCNode类的子类。
*/
bool BackScreen::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


	/// 定义一个节点
	CCNode *anode = CCNode::create();
	/// 将节点作为子节点加入场景类中；
	this->addChild(anode, 0);

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(BackScreen::menuCloseCallback));

	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
		origin.y + pCloseItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	/// 将菜单作为子节点加入anode节点中； 
	anode->addChild(pMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);

	// position the label on the center of the screen
	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pLabel->getContentSize().height));

	// add the label as a child to this layer
	anode->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::create("PIC_BJ_ZCJ_XH1.png");

	// position the sprite on the center of the screen
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	/// 将菜单作为子节点加入anode节点中；
	anode->addChild(pSprite, 0);
	
	/// 设置节点位置
	anode->setPosition(ccp(50, 50));

	/// 设置缩放
	anode->setScale(0.5);

	/// 设置旋转
	
	anode->setRotation(90.0);

	return true;
}