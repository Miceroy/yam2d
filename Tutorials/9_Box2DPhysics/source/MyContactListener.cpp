
#include "MyContactListener.h"

MyContactListener::MyContactListener() : m_contacts() {
}

MyContactListener::~MyContactListener() {
}

void MyContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
	b2Manifold* manifold = contact->GetManifold();
	manifold->localPoint;
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    m_contacts.push_back(myContact);
}

void MyContactListener::EndContact(b2Contact* contact) {
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<MyContact>::iterator pos;
    pos = std::find(m_contacts.begin(), m_contacts.end(), myContact);
    if (pos != m_contacts.end()) {
        m_contacts.erase(pos);
    }
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

