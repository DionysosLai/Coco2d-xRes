/**
*@file ContactListener.h
*@brief box2d Åö×²¼ì²â
*@author DionysosLai£¬email: 906391500@qq.com
*@version 1.0
*@data 2015-1-7 18:09
*/

#ifndef __MY_CONTACT_LISTENER_H__
#define __MY_CONTACT_LISTENER_H__

#include <vector>
#include <algorithm>

#include "Box2D/Box2D.h"

struct MyContact {
	b2Fixture *fixtureA;
	b2Fixture *fixtureB;
	bool operator==(const MyContact& other) const
	{
		bool isEquare = false;
		if ((fixtureA == other.fixtureA) && (fixtureB == other.fixtureB))
		{
			isEquare = true;
		}
		if ((fixtureA == other.fixtureB) && (fixtureB == other.fixtureA))
		{
			isEquare = true;
		}
		return isEquare;
	}

// 	bool operator==(const MyContact& other) const
// 	{
// 		bool isEquare = false;
// 		if ((fixtureA->GetBody()->GetUserData() == other.fixtureA->GetBody()->GetUserData())
// 			&&(fixtureB->GetBody()->GetUserData() == other.fixtureB->GetBody()->GetUserData()))
// 		{
// 			isEquare = true;
// 		}
// 		if ((fixtureA->GetBody()->GetUserData() == other.fixtureB->GetBody()->GetUserData())
// 			&&(fixtureB->GetBody()->GetUserData() == other.fixtureA->GetBody()->GetUserData()))
// 		{
// 			isEquare = true;
// 		}
// 		 
// 		return isEquare;
// 	}
};

class ContactListener : public b2ContactListener 
{
public:
	std::vector<MyContact> m_contacts;

	ContactListener();
	~ContactListener();

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	virtual void eraseBody(b2Body* body);
};

#endif	///< (__MY_CONTACT_LISTENER_H__)

