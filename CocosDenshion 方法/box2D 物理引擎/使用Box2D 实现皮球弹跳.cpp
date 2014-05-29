.h 
	CCSprite* ball;
	b2World* world;
	b2Body* body;
.cpp
#include "HelloWorldScene.h"

USING_NS_CC;

/// /// 定义了一个“像素/米”的比率。当你在cocos2d里面指定一个body在哪个位置时，你使用的单位要是米。但是，
/// 我们之前使用的都是像素作为单位，那样的话，位置就会不正确。根据Box2d参考手册，Box2d在处理大小在0.1
/// 到10个单元的对象的时候做了一些优化。这里的0.1米大概就是一个杯子那么大，10的话，大概就是一个箱子的大小。

/// 因此，我们并不直接传递像素，因为一个很小的对象很有60×60个像素，那已经大大超过了box2d优化时所限定的大小。
/// 因此，如果我们有一个64像素的对象，我们可以把它除以PTM_RATIO，得到2米---这个长度，box2d刚好可以很好地用来做物理仿真。
static const float PTM_RATIO = 32.0f;


CCScene* HelloWorld::scene()
{

    CCScene *scene = CCScene::create();

    HelloWorld *layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	this->setKeypadEnabled(true);

// 	CCGrid3DAction *grid = CCWaves3D::create(50, ccp(5, 5), 40, 10);
// 	this->runAction(grid    /////////////////////////////
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
    this->addChild(pLabel, 1);

    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(pSprite, 0);

	ball = CCSprite::create("ball.png");
	ball->setPosition(ccp(visibleSize.width/2.0f, visibleSize.height/2.0f));
	addChild(ball, 1);

	/// 创建世界
	b2Vec2 gravity(0.0f,-30.0f);   
	world = new b2World(gravity);  
	world->SetAllowSleeping(true);		///< 允许世界中的刚体休眠(当不发生碰撞时，即进入“休眠”);
	world->SetContinuousPhysics(true);	///< 允许持续的物体模拟

	/// 创建边界-即创建地面盒
	b2BodyDef groundBodyDef;  
	groundBodyDef.position.SetZero();	///< body置于左下角
	b2Body *groundBody = world->CreateBody(&groundBodyDef);		///< 使用world对象创建，不能直接new，因为world对象会做一些内存管理操作。  
	b2EdgeShape groundBox;				///< 为屏幕的每一个边界创建一个多边形shape。这些“shape”仅仅是一些线段。注意，我们把像素转换成了“meter”。通过除以之前定义的比率来实现的。
	b2FixtureDef boxShapeDef;			///< 一个fixture定义，指定shape为Edge shape
	boxShapeDef.shape = &groundBox;
	/// body对象来为每一个shape创建一个fixture对象 注意：一个body对象可以包含许许多多的fixture对象。
	groundBox.Set(b2Vec2(0,0),b2Vec2(visibleSize.width/PTM_RATIO,0));
	groundBody->CreateFixture(&boxShapeDef);
	groundBox.Set(b2Vec2(0,0), b2Vec2(0, visibleSize.height/PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);
	groundBox.Set(b2Vec2(0, visibleSize.height/PTM_RATIO), 
		b2Vec2(visibleSize.width/PTM_RATIO, visibleSize.height/PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);
	groundBox.Set(b2Vec2(visibleSize.width/PTM_RATIO,
		visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef); 

	// Create ball body and shape 创建球的body
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;	///< 指定body的类型为dynamic body。默认值是static body，那意味着那个body不能被移动也不会参与仿真。
	ballBodyDef.position.Set(visibleSize.width/2.0f/PTM_RATIO, visibleSize.height/2.0f/PTM_RATIO);
	ballBodyDef.userData = ball;		///< 设置body的user data属性为篮球精灵。可以设置任何东西，但是，你设置成精灵会很方便，特别是当两个body碰撞的时候，你可以通过这个参数把精灵对象取出来，然后做一些逻辑处理。
	body = world->CreateBody(&ballBodyDef);

	b2CircleShape circle;
	circle.m_radius = ball->getContentSize().height/2.0f/PTM_RATIO;

	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;	
	ballShapeDef.density = 1.0f;	///< 单位体积的质量（密度）。因此，一个对象的密度越大，那么它就有更多的质量，当然就会越难以移动. 
	ballShapeDef.friction = 0.2f;	///< 就是摩擦力。它的范围是0-1.0， 0意味着没有摩擦，1代表最大摩擦，几乎移不动的摩擦。	
	ballShapeDef.restitution = 0.8f;	///< 回复力。它的范围也是0到1.0. 0意味着对象碰撞之后不会反弹，1意味着是完全弹性碰撞，会以同样的速度反弹。
	body->CreateFixture(&ballShapeDef);
   
	scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt)  
{  
	/// 第一件事情就是调用world对象的step方法，这样它就可以进行物理仿真了。
	/// 这里的两个参数分别是“速度迭代次数”和“位置迭代次数”--你应该设置他们的范围在8-10之间。（译者：这里的数字越小，
	/// 精度越小，但是效率更高。数字越大，仿真越精确，但同时耗时更多。8一般是个折中，如果学过数值分析，应该知道迭代步数的具体作用）。
	world->Step(dt, 10, 10);

	/// 使我们的精灵匹配物理仿真。
	for (b2Body *b = world->GetBodyList(); b; b = b->GetNext())
	{
		/// 遍历world对象里面的所有body，然后看body的user data属性是否为空，如果不为空，就可以强制转换成精灵对象。接下来，就可以根据body的位置来更新精灵的位置了。
		if (b->GetUserData() != NULL)
		{
			CCSprite* ballData = (CCSprite*)b->GetUserData();
			ballData->setPosition(ccp(b->GetPosition().x*PTM_RATIO, 
				b->GetPosition().y*PTM_RATIO));
			/// 弧度值转化为角度值 cocos2d里面使用的是角度值，而box2d里面使用的是弧度值。单位不统一，所以需要转化。为什么前面*-1呢？
			/// 因为cocos2d的角度是顺时针，而box2d是逆时针。
			ballData->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
} 