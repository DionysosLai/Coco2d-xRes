/**
*@file ContactListener.cpp
*@brief Åö×²¼ì²â
*@author DionysosLai£¬email: 906391500@qq.com
*@version 1.0
*@data 2015-1-7 18:11
*/
#include "ContactListener.h"

ContactListener::ContactListener() : m_contacts() 
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(b2Contact* contact) 
{
	// We need to copy out the data because the b2Contact passed in is reused.
	if (NULL == contact->GetFixtureA()->GetBody() && NULL == contact->GetFixtureB()->GetBody())
	{
		return;
	}
	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	/// ¶ÔÊÀ½çÅö×²²»¼ì²â
	if (NULL != contact->GetFixtureA()->GetBody()->GetUserData() && NULL != contact->GetFixtureB()->GetBody()->GetUserData())
	{
		if (0 == m_contacts.size())
		{
			m_contacts.push_back(myContact);
		}
		else
		{
			for (unsigned int i = 0; i < m_contacts.size(); ++i)
			{
				MyContact contact = m_contacts.at(i);
				if (NULL != contact.fixtureA->GetBody() && NULL != contact.fixtureB->GetBody())
				{
					if (!(contact == myContact))
					{
						m_contacts.push_back(myContact);
						break;
					}
				}
			}
		}
	}
	
}

void ContactListener::EndContact(b2Contact* contact) 
{
	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	std::vector<MyContact>::iterator pos;
	pos = std::find(m_contacts.begin(), m_contacts.end(), myContact);
	if (pos != m_contacts.end()) {
		m_contacts.erase(pos);
	}
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
{

}

void ContactListener::eraseBody( b2Body* body )
{
	for (unsigned int i = 0; i < m_contacts.size(); ++i)
	{
		MyContact contact = m_contacts.at(i);
		if (contact.fixtureA->GetBody() == body || contact.fixtureB->GetBody() == body)
		{
			std::vector<MyContact>::iterator pos;
			pos = std::find(m_contacts.begin(), m_contacts.end(), contact);
			m_contacts.erase(pos);
		}
	}

}
