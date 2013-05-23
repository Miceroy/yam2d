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
#include "Enemy.h" // Include Enemy class header

#include "Input.h"
#include "Player.h"
#include "es_util.h"

using namespace yam2d; // Use namespace yam3d implicitily.



Enemy::Enemy(int gameObjectType, Texture* texture, Player* player)
	: SpriteGameObject(gameObjectType,texture) // Initalize base class by giving parameres to it
	, m_player(player)
{
}


Enemy::~Enemy(void)
{
}

void Enemy::update( float deltaTime )
{
	// If all waypoints reached or collides to player, do not move.
	if( hasReachedGoal() || collidesTo(m_player) )
		return; // nothing to do.

	float rotationSpeed = 1.0f; // Radians / second
	float moveSpeed = 4.0f; // tiles / second
	
	// Take last waypoint.
	vec2 destination = m_waypoints[m_waypoints.size()-1];

	// Get position delta from came object pos to wp pos.
	vec2 delta = destination-getPosition();
	float deltaLen = delta.Length();
	
	// If closer than 0.1 tiles from destination, we are close enought, so remove waypoint and return.
	if( deltaLen < 0.1f )
	{
		m_waypoints.pop_back();
		return;
	}
	
	// Make delta lenght to 1 by dividing delta by it length 
	delta *= 1.0f/deltaLen;

	// Take angle, which we need to rotate object.
	float angleDelta = atan2(delta.y,delta.x) - getRotation();

	// Enemy can move, if it is aligned close enought to target position
	bool canMoveForward = false;
	if( fabs(angleDelta) < 0.0001f )
	{
		canMoveForward = true;
	}

	// If current absoluteValue(angleDelta) > rotationSpeed*dt, then we would over turn in this frame, so limit rotation according to rotation speed.
	if( fabsf(angleDelta) > rotationSpeed*deltaTime )
	{
		float sign = (angleDelta >= 0.0f) ? 1.0f :  -1.0f;
		angleDelta = sign*rotationSpeed*deltaTime;
	}

	// Rotate gameobject accorging to angular velocity
	setRotation(getRotation() + angleDelta ); // Update rotation
	
	// If we are aligned to object, we can move towards it
	if( canMoveForward )
	{
		// Rotate forward direction according to game object rotation
		vec2 direction = rotateVector( vec2(moveSpeed,0), getRotation() );
		direction.Normalize(); // Make sure that lenght of direction vector is 1

		// Update position (euler integration)
		setPosition(getPosition() + deltaTime*moveSpeed*direction );
	}
}



void Enemy::setWayoints(const std::vector<vec2>& waypoints )
{
	// Make waypoints to reverse order for efficient use of pop_back in update.
	m_waypoints = waypoints;
	std::reverse(m_waypoints.begin(),m_waypoints.end());
}

bool Enemy::hasReachedGoal()
{
	// If there are no more waypoints, we have reached our goal.
	return m_waypoints.size() == 0;
}
