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
#include "Player.h" // Include Player class header

#include "Input.h"

using namespace yam2d; // Use namespace yam3d implicitily.



Player::Player(int gameObjectType, Texture* texture)
	: SpriteGameObject(gameObjectType,texture) // Initalize base class by giving parameres to it
{
}


Player::~Player(void)
{
}

void Player::update( float deltaTime )
{
	float rotationSpeed = 1.0f; // Radians / second
	float moveSpeed = 4.0f; // tiles / second

	// Rotate gameobject accorging to keys
	float rotate = float(getKeyState(KEY_RIGHT)-getKeyState(KEY_LEFT));
	setRotation(getRotation() + deltaTime*rotate*rotationSpeed ); // Update rotation

	// Get move direction from keyboard
	float forward = float(getKeyState(KEY_UP)-getKeyState(KEY_DOWN));

	if( fabsf(forward) > 0.1f )
	{
		// Rotate forward direction according to game object rotation
		vec2 direction = rotateVector( vec2(forward,0), getRotation() );
		direction.Normalize(); // Make sure that lenght of direction vector is 1

		// Update position (euler integration)
		setPosition(getPosition() + deltaTime*moveSpeed*direction );
	}

}

