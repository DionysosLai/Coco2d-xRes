/**
*@file Box2dCircle.h
*@brief http://www.hakim.se/experiments/html5/blob/03/
*@author DionysosLai£¬email: 906391500@qq.com
*@version 1.0
*@data 2014-11-5 17:35
*/
#ifndef __BOX2D_CIRCLE_SCENE_H__
#define __BOX2D_CIRCLE_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class Box2dCircle : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	static cocos2d::CCScene* scene();
	void menuCloseCallback(CCObject* pSender);

	CREATE_FUNC(Box2dCircle);

	virtual void draw();

	void creatWord();
	void createrMatter();

	virtual void update(float delta);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

private: 
	b2World* m_world;
	b2Body* m_body[32];
	b2Body* m_bodyCenter;
	b2MouseJoint* m_mouseJoint;

	float m_fRadius;
public:
};

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;

};
#endif // __BOX2D_CIRCLE_SCENE_H__
