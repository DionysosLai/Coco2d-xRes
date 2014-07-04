#include "HelloWorldScene.h"

USING_NS_CC;

static const float PTM_RATIO = 32.0f;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	creatWord();
	addMatter();

	this->scheduleUpdate();

    m_mouseJoint = NULL;

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::creatWord( void )
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	/// 创建世界
	b2Vec2 gravity(0.0f,-30.0f);   
	m_world = new b2World(gravity);  
	m_world->SetAllowSleeping(true);		///< 允许世界中的刚体休眠(当不发生碰撞时，即进入“休眠”);
	m_world->SetContinuousPhysics(true);	///< 允许持续的物体模拟

	/// 创建边界-即创建地面盒
	b2BodyDef groundBodyDef;  
	groundBodyDef.position.SetZero();	///< body置于左下角
	b2Body *groundBody = m_world->CreateBody(&groundBodyDef);		///< 使用world对象创建地面物体，不能直接new，因为world对象会做一些内存管理操作。  
	b2EdgeShape groundBox;				///< 为屏幕的每一个边界创建一个多边形shape。这些“shape”仅仅是一些线段。注意，我们把像素转换成了“meter”。通过除以之前定义的比率来实现的。
	//	b2PolygonShape groundBox;
	b2FixtureDef boxShapeDef;			///< 一个fixture定义，指定shape为Edge shape
	// 	boxShapeDef.filter.categoryBits	= BOX2D_MASK_BALL | BOX2D_MASK_BASKET;
	// 	boxShapeDef.filter.maskBits		= BOX2D_MASK_BALL | BOX2D_MASK_BASKET;
	boxShapeDef.shape = &groundBox;
	/// body对象来为每一个shape创建一个fixture对象 注意：一个body对象可以包含许许多多的fixture对象。
	groundBox.Set(b2Vec2(0,0), b2Vec2(visibleSize.width/PTM_RATIO,0));
	groundBody->CreateFixture(&boxShapeDef);
	groundBox.Set(b2Vec2(0,0), b2Vec2(0, visibleSize.height/PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);
	groundBox.Set(b2Vec2(0, visibleSize.height/PTM_RATIO), 
		b2Vec2(visibleSize.width/PTM_RATIO, visibleSize.height/PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);
	groundBox.Set(b2Vec2(visibleSize.width/PTM_RATIO,
		visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);
}

void HelloWorld::addMatter()
{
	// Create ball body and shape 创建球的body
	CCSprite* ball = CCSprite::create("PIC_YLYZ.png");
	ball->setPosition(ccp(640, 400));
	this->addChild(ball, 1);
// 
// 	b2PolygonShape shape;
// 	shape.SetAsBox(0.6f, 0.125f);
// 
// 	b2FixtureDef fd;
// 	fd.shape = &shape;
// 	fd.density = 20.0f;
// 	fd.friction = 0.2f;
// 
// 	b2RevoluteJointDef jd;
// 	jd.collideConnected = false;
// 
// 	const float32 y = 25.0f;
// 	b2Body* prevBody = ground;
// 	for (int32 i = 0; i < 10; ++i)
// 	{
// 		b2BodyDef bd;
// 		bd.type = b2_dynamicBody;
// 		bd.position.Set(0.5f + i, y);
// 		b2Body* body = m_world->CreateBody(&bd);
// 		body->CreateFixture(&fd);
// 
// 		b2Vec2 anchor(float32(i), y);
// 		jd.Initialize(prevBody, body, anchor);
// 		m_world->CreateJoint(&jd);
// 
// 		prevBody = body; 
// 	}


	b2Body *body;

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;	///< 指定body的类型为dynamic body。默认值是static body，那意味着那个body不能被移动也不会参与仿真。
	ballBodyDef.position.Set(640/PTM_RATIO, 400/PTM_RATIO);
	ballBodyDef.userData = ball;		///< 设置body的user data属性为篮球精灵。可以设置任何东西，但是，你设置成精灵会很方便，特别是当两个body碰撞的时候，你可以通过这个参数把精灵对象取出来，然后做一些逻辑处理。
	body = m_world->CreateBody(&ballBodyDef);

	b2CircleShape circle;
	circle.m_radius = ball->getContentSize().width/2.0f/PTM_RATIO;

	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	// 		ballShapeDef.filter.categoryBits	= BOX2D_MASK_BALL;
	// 		ballShapeDef.filter.maskBits		= BOX2D_MASK_BALL;
	ballShapeDef.density = 1.0f;		///< 单位体积的质量（密度）。因此，一个对象的密度越大，那么它就有更多的质量，当然就会越难以移动. 
	ballShapeDef.friction = 0.2f;		///< 就是摩擦力。它的范围是0-1.0， 0意味着没有摩擦，1代表最大摩擦，几乎移不动的摩擦。	
	ballShapeDef.restitution = 0.5f;		///< 回复力。它的范围也是0到1.0. 0意味着对象碰撞之后不会反弹，1意味着是完全弹性碰撞，会以同样的速度反弹。
	body->CreateFixture(&ballShapeDef);
}

void HelloWorld::update( float delta )
{
	m_world->Step(delta, 10, 10);

	/// 使我们的精灵匹配物理仿真。
	for (b2Body *b = m_world->GetBodyList(); b; )
	{

		///遍历world对象里面的所有body，然后看body的user data属性是否为空，如果不为空，就可以强制转换成精灵对象。接下来，就可以根据body的位置来更新精灵的位置了。
		if (b->GetUserData() != NULL)
		{
			b2Body* b2node = b;
			b = b2node->GetNext();
			CCSprite* ballData = (CCSprite*)b2node->GetUserData();
			ballData->setPosition(ccp(b2node->GetPosition().x*PTM_RATIO, 
				b2node->GetPosition().y*PTM_RATIO));
			ballData->setRotation(-1*CC_RADIANS_TO_DEGREES(b2node->GetAngle()));
		}
		else
		{
			b = b->GetNext();
		}
	}
}

bool HelloWorld::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint point = pTouch->getLocation();

	do 
	{
		b2Vec2 vec(point.x/PTM_RATIO,point.y/PTM_RATIO);
		//	b2Vec2 vec = b2Vec2(touchPoint.x,touchPoint.y);

		if(m_mouseJoint != NULL)
		{
			return false;
		}

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = vec - d;
		aabb.upperBound = vec + d;

		b2BodyDef bodyDef;
		b2Body *m_groundBody = m_world->CreateBody(&bodyDef);

		// Query the world for overlapping shapes.
		QueryCallback callback(vec);
		m_world->QueryAABB(&callback, aabb);

		if (callback.m_fixture)
		{
			b2Body* body = callback.m_fixture->GetBody();
			b2MouseJointDef md;
			md.bodyA = m_groundBody;//一般为世界边界    
			md.bodyB = body;//需要拖动的物体 
			md.target = vec;
			md.maxForce = 1000.0f * body->GetMass();
			m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
			body->SetAwake(true);

			//		CCLog("touch bengin \n");

			return true;
		}
		return false;
	} while (0);
}

void HelloWorld::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint point = pTouch->getLocation();

	do 
	{

		b2Vec2 vecMouse;  
		vecMouse.Set((point.x)/PTM_RATIO, (point.y)/PTM_RATIO);  
		//改变关节位置.  
		m_mouseJoint->SetTarget(vecMouse);
	} while (0);


	if(m_mouseJoint == NULL )  
		return;  
}

void HelloWorld::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint point = pTouch->getLocation();

	do 
	{
		//销毁关节.  
		if(m_mouseJoint != NULL)  
		{  
			m_world->DestroyJoint(m_mouseJoint);  
			m_mouseJoint  =NULL;  
		}  
	} while (0);
}

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void HelloWorld::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}