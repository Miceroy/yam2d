// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef PHYSICS_BODY_H_
#define PHYSICS_BODY_H_ 

// Include base class
#include <GameObject.h>
#include <Box2D\Box2D.h>

class PhysicsBody : public yam2d::Component, public yam2d::Updatable
{
public:
	/// Create dynamic Physics body (controlled by forces)
	// linearDamping = 2.0f, float angularDamping = 8.0f
	PhysicsBody(yam2d::GameObject* parent, b2World* world, float linearDamping, float angularDamping);
	/// Create static Physics body
	PhysicsBody(yam2d::GameObject* parent, b2World* world);

	virtual ~PhysicsBody(void);

	void setBoxFixture(const yam2d::vec2& size, const yam2d::vec2& center, float angle, bool isSensor, float density = 1.0f, float restitution = 1.0f, float friction = 0.9f);

	void setCircleFixture(float radius, bool isSensor, float density = 1.0f, float restitution = 1.0f, float friction = 0.9f);
	
	b2Body* getBody() { return m_body; }

	virtual void update(float deltaTime);

	yam2d::GameObject* getGameObject() { return (yam2d::GameObject*)getOwner(); }
	const yam2d::GameObject* getGameObject() const { return (const yam2d::GameObject*)getOwner(); }
	
private:
	yam2d::Ref<b2World> m_world;
	b2Body* m_body;

};


#endif