#include "HelloWorldScene.h"
#include "CircleActionBy.h"

USING_NS_CC;


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
  
    return true;
}

void HelloWorld::onEnter()
{
}

void HelloWorld::draw()
{
	ccDrawColor4B(255, 0, 0, 0);
	glLineWidth(1);
	drawLighting(100, 300, 800, 300, ccpDistance(ccp(100, 300), ccp(800, 300)));
	ccDrawColor4B(0, 0, 0, 0);
	glLineWidth(1);
}

void HelloWorld::drawLighting( float x1, float y1, float x2, float y2, float displace )
{
	float curDettail = 5.f;
	if (displace < curDettail)
	{
		ccDrawLine(ccp(x1, y1), ccp(x2, y2));
	}
	else
	{
		float mid_x = (x2 + x1) / 2.f;
		float mid_y = (y2 + y1) / 2.f;
		mid_x += (CCRANDOM_0_1() - .5f) * displace;
		mid_y += (CCRANDOM_0_1() - .5f) * displace;
		drawLighting(x1, y1, mid_x, mid_y, displace/2.f);
		drawLighting(x2, y2, mid_x, mid_y, displace/2.f);
	}
}
