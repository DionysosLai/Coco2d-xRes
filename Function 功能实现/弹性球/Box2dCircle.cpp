/**
*@file Box2dCircle.cpp
*@brief http://www.hakim.se/experiments/html5/blob/03/
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-11-5 17:42
*/
#include "Box2dCircle.h"
#include "Geometry.h"

USING_NS_CC;

static const float PTM_RATIO = 32.0f;

CCScene* Box2dCircle::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	Box2dCircle *layer = Box2dCircle::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Box2dCircle::init()
{

	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	m_fRadius = 150.f;
	creatWord();
	createrMatter();
	m_mouseJoint = NULL;

	this->scheduleUpdate();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	

// 	CCSprite* ball1 = CCSprite::create("qiu.png");
// 	ball1->setPosition(ccp(340, 400));
// 	this->addChild(ball1);

	return true;
}

void Box2dCircle::draw()
{
//	CCLayer::draw();
//	m_world->DrawDebugData();
}

void Box2dCircle::creatWord()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	/// 创建世界
	b2Vec2 gravity(0.0f,-35.0f);   
	m_world = new b2World(gravity);  
	m_world->SetAllowSleeping(true);		///< 允许世界中的刚体休眠(当不发生碰撞时，即进入“休眠”);
	m_world->SetContinuousPhysics(true);	///< 允许持续的物体模拟

// 	m_debugDraw = new GLESDebugDraw( PTM_RATIO );
// 
// 	uint32 flags = 0;
// 	flags += b2Draw::e_shapeBit;
// 	flags += b2Draw::e_jointBit;
// 	flags += b2Draw::e_aabbBit;
// 	flags += b2Draw::e_pairBit;
// 	flags += b2Draw::e_centerOfMassBit;
// 	m_debugDraw->SetFlags(flags);
// 
// 	m_world->SetDebugDraw(m_debugDraw);



	/// 创建边界-即创建地面盒
	b2BodyDef groundBodyDef;  
	groundBodyDef.position.SetZero();	///< body置于左下角
	b2Body *groundBody = m_world->CreateBody(&groundBodyDef);		///< 使用world对象创建地面物体，不能直接new，因为world对象会做一些内存管理操作。  
	b2EdgeShape groundBox;				///< 为屏幕的每一个边界创建一个多边形shape。这些“shape”仅仅是一些线段。注意，我们把像素转换成了“meter”。通过除以之前定义的比率来实现的。
	//	b2PolygonShape groundBox;
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
}

void Box2dCircle::createrMatter()
{
	// Create ball body and shape 创建球的body
	const float coef		= 2.0f * (float)M_PI/32;	///< 计算每两个相邻顶点与中心的夹角
	for (unsigned short i = 0; i < 32; ++i)
	{
//  		CCSprite* ball = CCSprite::create("eraser2.png");
//  		ball->setScale(0.2f);
		CCPoint pos = CCPointZero;
		float rads	= i*coef;//弧度
		pos.x = 640.f + m_fRadius*cosf(rads);
		pos.y = 200.f + m_fRadius*sinf(rads);
// 		ball->setPosition(pos);
// 		this->addChild(ball, -1);

		b2BodyDef ballBodyDef;
		ballBodyDef.type = b2_dynamicBody;		///< 指定body的类型为dynamic body。默认值是static body，那意味着那个body不能被移动也不会参与仿真。
		ballBodyDef.position.Set(pos.x/PTM_RATIO, pos.y/PTM_RATIO);
		
		m_body[i] = m_world->CreateBody(&ballBodyDef);
		b2CircleShape circle;
		circle.m_radius = 1.f/PTM_RATIO;
		
		b2FixtureDef ballShapeDef;
		ballShapeDef.shape = &circle;
		ballShapeDef.density = 1.0f;		///< 单位体积的质量（密度）。因此，一个对象的密度越大，那么它就有更多的质量，当然就会越难以移动. 
		ballShapeDef.friction = 1.0f;		///< 就是摩擦力。它的范围是0-1.0， 0意味着没有摩擦，1代表最大摩擦，几乎移不动的摩擦。	
		ballShapeDef.restitution = 1.f;		///< 回复力。它的范围也是0到1.0. 0意味着对象碰撞之后不会反弹，1意味着是完全弹性碰撞，会以同样的速度反弹。
		m_body[i]->CreateFixture(&ballShapeDef);
	}

	CCSprite* ball = CCSprite::create("qiu.png");
	ball->setScale(150/150);
	ball->setPosition(ccp(640, 200));
	this->addChild(ball, -1);

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;		///< 指定body的类型为dynamic body。默认值是static body，那意味着那个body不能被移动也不会参与仿真。
	ballBodyDef.position.Set(640.f/PTM_RATIO, 200.f/PTM_RATIO);
	ballBodyDef.userData = ball;
	m_bodyCenter = m_world->CreateBody(&ballBodyDef);
	b2CircleShape circle;
	circle.m_radius = 1.2f/PTM_RATIO;

	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	ballShapeDef.density = 1.0f;		///< 单位体积的质量（密度）。因此，一个对象的密度越大，那么它就有更多的质量，当然就会越难以移动. 
	ballShapeDef.friction = 1.0f;		///< 就是摩擦力。它的范围是0-1.0， 0意味着没有摩擦，1代表最大摩擦，几乎移不动的摩擦。	
	ballShapeDef.restitution = 0.5f;		///< 回复力。它的范围也是0到1.0. 0意味着对象碰撞之后不会反弹，1意味着是完全弹性碰撞，会以同样的速度反弹。
	m_bodyCenter->CreateFixture(&ballShapeDef);

#if 1
	for (short i = 0; i < 32; ++i)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body[i%32], m_body[(i+1)%32], m_body[i%32]->GetWorldCenter(), m_body[(i+1)%32]->GetWorldCenter());
		// 		jointDef.Initialize(m_body[i%32], m_body[(i+2)%32], m_body[i%32]->GetPosition(), m_body[(i+2)%32]->GetPosition());
		// 		jointDef.Initialize(m_body[i%32], m_body[(i+3)%32], m_body[i%32]->GetPosition(), m_body[(i+3)%32]->GetPosition());
		// 		jointDef.Initialize(m_body[i%32], m_body[(i-1+32)%32], m_body[i%32]->GetPosition(), m_body[(i-1+32)%32]->GetPosition());
		// 		jointDef.Initialize(m_body[i%32], m_body[(i-2+32)%32], m_body[i%32]->GetPosition(), m_body[(i-2+32)%32]->GetPosition());
		// 		jointDef.Initialize(m_body[i%32], m_body[(i-3+32)%32], m_body[i%32]->GetPosition(), m_body[(i-3+32)%32]->GetPosition());
		//		jointDef.Initialize(m_body[i%32], m_bodyCenter, m_body[i%32]->GetWorldCenter(), m_bodyCenter->GetWorldCenter());
		jointDef.collideConnected = true;
		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
	}
	for (short i = 0; i < 32; ++i)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body[i%32], m_bodyCenter, m_body[i%32]->GetWorldCenter(), m_bodyCenter->GetWorldCenter());
		jointDef.collideConnected = true;
		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
	}
#else
	for (short i = 0; i < 32; ++i)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body[i%32], m_body[(i+1)%32], m_body[i%32]->GetWorldCenter(), m_body[(i+1)%32]->GetWorldCenter());
		jointDef.collideConnected = true;
		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
	}
	for (short i = 0; i < 32; ++i)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body[i%32], m_body[(i+2)%32], m_body[i%32]->GetPosition(), m_body[(i+2)%32]->GetPosition());
		jointDef.collideConnected = true;
		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
	}
	for (short i = 0; i < 32; ++i)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(m_body[i%32], m_body[(i+3)%32], m_body[i%32]->GetPosition(), m_body[(i+3)%32]->GetPosition());
		jointDef.collideConnected = true;
		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
	}
// 	for (short i = 0; i < 32; ++i)
// 	{
// 		b2DistanceJointDef jointDef;
// 		jointDef.Initialize(m_body[i%32], m_body[(i-1)%32], m_body[i%32]->GetPosition(), m_body[(i-1)%32]->GetPosition());
// 		jointDef.collideConnected = true;
// 		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
// 	}
// 	for (short i = 0; i < 32; ++i)
// 	{
// 		b2DistanceJointDef jointDef;
// 		jointDef.Initialize(m_body[i%32], m_body[(i-2+32)%32], m_body[i%32]->GetPosition(), m_body[(i-2+32)%32]->GetPosition());
// 		jointDef.collideConnected = true;
// 		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
// 	}
// 	for (short i = 0; i < 32; ++i)
// 	{
// 		b2DistanceJointDef jointDef;
// 		jointDef.Initialize(m_body[i%32], m_body[(i-3+32)%32], m_body[i%32]->GetPosition(), m_body[(i-3+32)%32]->GetPosition());
// 		jointDef.collideConnected = true;
// 		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
// 	}
// 	for (short i = 0; i < 32; ++i)
// 	{
// 		b2DistanceJointDef jointDef;
// 		jointDef.Initialize(m_body[i%32], m_bodyCenter, m_body[i%32]->GetWorldCenter(), m_bodyCenter->GetWorldCenter());
// 		jointDef.collideConnected = true;
// 		b2DistanceJointDef* distanceJoint = (b2DistanceJointDef*)m_world->CreateJoint(&jointDef);
// 	}
#endif
}

void Box2dCircle::update( float delta )
{
	m_world->Step(delta, 10, 10);

	/// 使我们的精灵匹配物理仿真。
	CCSprite* ballData = (CCSprite*)m_bodyCenter->GetUserData();
	ballData->setPosition(ccp(m_bodyCenter->GetPosition().x*PTM_RATIO, 
		m_bodyCenter->GetPosition().y*PTM_RATIO));
	ballData->setRotation(-1*CC_RADIANS_TO_DEGREES(m_bodyCenter->GetAngle()));

	/// 施加力
	CCPoint posCenter = CCPointZero;
	posCenter.x = m_bodyCenter->GetPosition().x*PTM_RATIO;
	posCenter.y = m_bodyCenter->GetPosition().y*PTM_RATIO;
// 	for (short i = 0; i < 32; ++i)
// 	{
// 		CCPoint pos1 = CCPointZero, pos2 = CCPointZero;
// 		pos1.x = m_body[i]->GetPosition().x*PTM_RATIO;
// 		pos1.y = m_body[i]->GetPosition().y*PTM_RATIO;
// 		
// 		float dis = ccpDistance(pos1, posCenter);
// 		dis -= m_fRadius;
// 
// 		if (dis < -1.f)
// 		{
// 			dis = -dis;
// 			/// 获取角度+施加力
// 			float angle = getAngle(posCenter, pos1);
// 			b2Vec2 vec;
// 			CCPoint posAng = ccpNormalize(pos1-posCenter);
// 			vec.x = posAng.x*20;
// 			vec.y = posAng.y*20;
// 			m_bodyCenter->ApplyForce(vec,m_bodyCenter->GetWorldCenter());
// 		}
// 	}
}

bool Box2dCircle::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint point = pTouch->getLocation();

	b2Vec2 vec(point.x/PTM_RATIO, point.y/PTM_RATIO);
	//	b2Vec2 vec = b2Vec2(touchPoint.x,touchPoint.y);

	if(m_mouseJoint != NULL)
	{
		return false;
	}

	b2Vec2 posVec = m_bodyCenter->GetPosition();
	CCPoint pos;
	pos.x = posVec.x * PTM_RATIO;
	pos.y = posVec.y * PTM_RATIO;

	if (ccpDistance(pos, point) < m_fRadius)
	{
		b2BodyDef bodyDef;
		b2Body *m_groundBody = m_world->CreateBody(&bodyDef);
		b2MouseJointDef md;
		md.bodyA = m_groundBody;//一般为世界边界    
		md.bodyB = m_bodyCenter;//需要拖动的物体 
		md.target = vec;
		md.maxForce = 20000.0f * m_bodyCenter->GetMass();
		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
		m_bodyCenter->SetAwake(true);
	
		return true;
	}

	// Make a small box.
// 	b2AABB aabb;
// 	b2Vec2 d;
// 	d.Set(0.001f, 0.001f);
// 	aabb.lowerBound = vec - d;
// 	aabb.upperBound = vec + d;
// 
// 	b2BodyDef bodyDef;
// 	b2Body *m_groundBody = m_world->CreateBody(&bodyDef);
// 
// 	// Query the world for overlapping shapes.
// 	QueryCallback callback(vec);
// 	m_world->QueryAABB(&callback, aabb);
// 
// 	if (callback.m_fixture)
// 	{
// 		b2Body* body = callback.m_fixture->GetBody();
// 		b2MouseJointDef md;
// 		md.bodyA = m_groundBody;//一般为世界边界    
// 		md.bodyB = body;//需要拖动的物体 
// 		md.target = vec;
// 		md.maxForce = 1000.0f * body->GetMass();
// 		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
// 		body->SetAwake(true);
// 		return true;
// 	}
	return false;
}

void Box2dCircle::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint point = pTouch->getLocation();
	b2Vec2 vecMouse;  
	vecMouse.Set((point.x)/PTM_RATIO, (point.y)/PTM_RATIO);  
	//改变关节位置.  
	m_mouseJoint->SetTarget(vecMouse);
}

void Box2dCircle::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	//销毁关节.  
	if(m_mouseJoint != NULL)  
	{  
		m_world->DestroyJoint(m_mouseJoint);  
		m_mouseJoint  =NULL;  
	}  
}
