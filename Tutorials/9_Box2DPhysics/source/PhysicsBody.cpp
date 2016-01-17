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
#include "PhysicsBody.h" // Include Enemy class header

#include "Input.h"
#include "es_util.h"

using namespace yam2d; // Use namespace yam3d implicitily.

namespace
{

	// dynamic: positive mass, non - zero velocity determined by forces, moved by solver
	b2Body* createDynamicBody(b2World* world, const yam2d::vec2& initialPosition, float initialRotation, void* userData, float linearDamping = 2.0f, float angularDamping = 8.0f)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(initialPosition.x, initialPosition.y);
		bodyDef.angle = initialRotation;
		bodyDef.userData = userData;
		bodyDef.bullet = true;
		bodyDef.linearDamping = linearDamping;
		bodyDef.angularDamping = angularDamping;
		bodyDef.allowSleep = false;
		bodyDef.awake = true;
		bodyDef.fixedRotation = false;
		return world->CreateBody(&bodyDef);
	}

	b2Body* createStaticBody(b2World* world, const yam2d::vec2& initialPosition, float initialRotation, void* userData)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(initialPosition.x, initialPosition.y);
		bodyDef.angle = initialRotation;
		bodyDef.userData = userData;
		bodyDef.bullet = false;
		bodyDef.allowSleep = true;
		bodyDef.awake = false;
		bodyDef.fixedRotation = true;
		return world->CreateBody(&bodyDef);
	}
	/*
	b2Fixture* setBoxFixture(b2Body* body, const yam2d::vec2& size, const yam2d::vec2& center, float angle, bool isSensor, float density = 1.0f, float restitution = 1.0f, float friction = 0.9f)
	{
		b2PolygonShape shape;
		shape.SetAsBox(size.x, size.y, center, angle);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.isSensor = isSensor;
		fixtureDef.density = density;
		fixtureDef.restitution = restitution;
		fixtureDef.friction = friction;
		return body->CreateFixture(&fixtureDef);
	}

	b2Fixture* setCircleFixture(b2Body* body, float radius, bool isSensor, float density = 1.0f, float restitution = 1.0f, float friction = 0.9f)
	{
		b2CircleShape shape;
		shape.m_radius = radius;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.isSensor = isSensor;
		fixtureDef.density = density;
		fixtureDef.restitution = restitution;
		fixtureDef.friction = friction;
		return body->CreateFixture(&fixtureDef);
	}*/
}


PhysicsBody::PhysicsBody(yam2d::GameObject* parent, b2World* world, float linearDamping, float angularDamping)
	: Component(parent, Component::getDefaultProperties())
	, m_world(world)
	, m_body(0)
{
	m_body = createDynamicBody(m_world, parent->getPosition(), parent->getRotation(), this, linearDamping, angularDamping);
	m_body->SetTransform(parent->getPosition(), parent->getRotation());
}

PhysicsBody::PhysicsBody(yam2d::GameObject* parent, b2World* world)
	: Component(parent, Component::getDefaultProperties())
	, m_world(world)
	, m_body(0)
{
	m_body = createStaticBody(m_world, parent->getPosition(), parent->getRotation(), this);
}

PhysicsBody::~PhysicsBody(void)
{
	m_world->DestroyBody(m_body);
}

void PhysicsBody::setBoxFixture(const yam2d::vec2& size, const yam2d::vec2& center, float angle, bool isSensor, float density, float restitution, float friction)
{
	b2PolygonShape shape;
	shape.SetAsBox(size.x/2, size.y/2, center, angle);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = isSensor;
	fixtureDef.density = density;
	fixtureDef.restitution = restitution;
	fixtureDef.friction = friction;
	m_body->CreateFixture(&fixtureDef);
}

void PhysicsBody::setCircleFixture(float radius, bool isSensor, float density, float restitution, float friction)
{
	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = isSensor;
	fixtureDef.density = density;
	fixtureDef.restitution = restitution;
	fixtureDef.friction = friction;
	m_body->CreateFixture(&fixtureDef);
}


void PhysicsBody::update(float deltaTime)
{
	if (m_body->GetType() == b2BodyType::b2_dynamicBody)
	{
		// Set position from body to gameObject
		b2Transform tr = m_body->GetTransform();
		yam2d::vec2 newPosition = yam2d::vec2(tr.p.x, tr.p.y);
		float newRotation = -tr.q.GetAngle();
		getGameObject()->setPosition(newPosition);
		getGameObject()->setRotation(newRotation);
	}
}