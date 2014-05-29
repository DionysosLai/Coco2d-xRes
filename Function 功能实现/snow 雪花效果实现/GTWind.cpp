#include "GTWind.h"

USING_NS_CC;

GTWind::GTWind(void)
{

}

GTWind::~GTWind(void)
{

}

void GTWind::showWind(void)
{
	//初始的飘动方向
	m_windDir = tag_wind_none;
	//重力的值，负数向下
	g = -4;
	//每一帧构造的雪花数
	maxV = 2;
	//当前屏幕最大雪花数
	maxCount = 150;
	//风速
	windVelocity = 0;
	winSize = CCDirector::sharedDirector()->getWinSize();
	//使用SpriteBatchNode初始化，让所有雪花共享一块内存，
	//而且只需要调用一次OPENGL绘画，就可以构造所有雪花，提高效率
	CCSpriteBatchNode *spriteBatchNode = CCSpriteBatchNode::create("snow.png",300);
	addChild(spriteBatchNode,0,tag_batch_node);
	schedule(schedule_selector(GTWind::changeWind),5.0f);
	schedule(schedule_selector(GTWind::updataWind));
}


void GTWind::changeWind(float dt)
{
	int dir = (int)(CCRANDOM_0_1() * 3) +1;
	if(m_windDir==dir)
	{
		return;
	}
	m_windDir=dir;

	switch (m_windDir)
	{
	case tag_wind_none:
		//规定雪花的最大速度
		maxWindVelocity = 0;
		//动态改变速度
		schedule(schedule_selector(GTWind::upDataWindVelocity));
		break;
	case tag_wind_left:
		maxWindVelocity = 20;
		schedule(schedule_selector(GTWind::upDataWindVelocity));
		break;
	case tag_wind_right:
		maxWindVelocity = -20;
		schedule(schedule_selector(GTWind::upDataWindVelocity));
		break;
	default:
		break;
	}
}

void GTWind::upDataWindVelocity(float dt)
{
	int addV = maxWindVelocity - windVelocity;

	if(abs(addV)>10)
	{
		windVelocity += addV/10;
	}
	else if(abs(addV)>2 &&abs(addV)<=10)
	{
		windVelocity += addV/abs(addV);
	}
	else
	{
		windVelocity = maxWindVelocity;
		unschedule(schedule_selector(GTWind::upDataWindVelocity));
	}
}

void GTWind::updataWind(float dt)
{
	CCSpriteBatchNode *temSpriteBatchNode = (CCSpriteBatchNode *)getChildByTag(tag_batch_node);
	if(temSpriteBatchNode->getChildren()->count()<maxCount)
	{
		for(int i=0; i< maxV; i++)
		{
			//从SpriteBatchNode读取贴图创建sprite，并加入到SpriteBatchNode
			CCSprite* sprite = CCSprite::createWithTexture(temSpriteBatchNode->getTexture());
			temSpriteBatchNode->addChild(sprite);
			sprite->setScale(1.0f - ((int)(CCRANDOM_0_1() * 5)+5) / 10.0);
			//初始化每个雪花的位置
			if (windVelocity >0)
				sprite->setPosition(ccp(winSize.width+10, 1.0*((int)(CCRANDOM_0_1() * ((int)winSize.height+200)))));
			else
				sprite->setPosition(ccp(-10,1.0*((int)(CCRANDOM_0_1() * ((int)winSize.height+200)))));

			if (windVelocity <3 && windVelocity > -3)
				sprite->setPosition(ccp(1.0*((int)(CCRANDOM_0_1() * (int)winSize.height+240)), winSize.height+200));
				
		}
	}
	/// 改变所有雪花属性
	CCArray* allSprite=temSpriteBatchNode->getChildren();
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(allSprite, pObject)
	{
		CCSprite* pChild = (CCSprite*) pObject;
		CCPoint pNow = pChild->getPosition();
// 		m_fSnowVelocityX += 0.01*windVelocity/pChild->getScale();
		pNow.x -= 0.08/pChild->getScale()*windVelocity;	///< 体积越小，偏移速度越快(这个效果好)
//		pNow.x -= pChild->getScale()*windVelocity;		///< 体积越小，偏移速度越慢
/*		pNow.x -= m_fSnowVelocityX + 0.1 * windVelocity;*/
		pNow.y += g;
		pChild->setPosition(pNow);
		pChild->setRotation(pChild->getRotation()+0.1f);

// 		if(pChild->getPosition().x < -10 || pChild->getPosition().x > winSize.width+10 ||
// 			pChild->getPosition().y < -10 ||)
		/// 在x轴上根据范围来判断雪花是否消失，在y轴上，根据雪花的大小来判断消失的位置，雪花越大，消失的y轴越小。
		if (pChild->getPosition().x < -10 || pChild->getPosition().x > winSize.width+10 ||
				pChild->getPosition().y < (200 - pChild->getScale()*400))	///< 200、400 根据自己的屏幕效果来调，这里的屏幕是320*480
		{
			if (windVelocity >0)
			{
/*				float randY = 1.0*(int)(CCRANDOM_0_1() * (int)(winSize.height+200) )*/
				/// 初始位置 风速大于0 在屏幕右边
				pChild->setPosition(ccp(winSize.width+10,	1.0*((int)(CCRANDOM_0_1() * ((int)winSize.height+200) )) ));
			}
			else
			{
				pChild->setPosition(ccp(-10,1.0*((int)(CCRANDOM_0_1() * ((int)winSize.height+200)))));
			}
			if (windVelocity <3 && windVelocity > -3)
			{
				pChild->setPosition(ccp( 1.0*((int)(CCRANDOM_0_1() * ((int)winSize.height+240))),winSize.height+10));
			}
			/// 当前x轴雪花速度为0;
// 			m_fSnowVelocityX = 0;
		}
	}
}