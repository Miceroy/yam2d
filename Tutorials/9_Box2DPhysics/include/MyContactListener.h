#ifndef MYCONTACTLISTENER_H_
#define MYCONTACTLISTENER_H_


#include <Box2d/Box2D.h>
#include <vector>
#include <algorithm>
#include <vec2.h>
#include <Object.h>

struct MyContact 
{
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;

    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class MyContactListener : public b2ContactListener, public yam2d::Object  {

public:
    std::vector<MyContact> m_contacts;
    
    MyContactListener();
    ~MyContactListener();
    
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
};


#endif

