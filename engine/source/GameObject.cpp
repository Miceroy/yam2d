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

#include <GameObject.h>

namespace yam2d
{


GameObject::GameObject(int type, const vec2& position, const vec2& size, const std::string& name )
: m_name(name)
, m_position(position)
, m_offset(0)
, m_topLeft(0.0f)
, m_bottomRight(0.0f)
, m_rotation(0.0f)
, m_size(size)
, m_tileScale(1.0f)
, m_type(type)
{
	recalcExtens();
}


GameObject::~GameObject()
{
}


void GameObject::update( float )
{
}

	
void GameObject::render( Layer* )
{
}


void GameObject::setName( const std::string& name )
{ 
	m_name = name; 
}


void GameObject::setPosition( const vec2& position )
{
	m_position = position; 
	recalcExtens();
} 


void GameObject::setRotation( float rotation )
{ 
	m_rotation = rotation; 
} 


void GameObject::setSize( float sizeX, float sizeY ) 
{ 
	setSize(vec2(sizeX,sizeY));
}


void GameObject::setSize( const vec2& size ) 
{ 
	assert(size.x >= 0.0f && size.y >= 0.0f); 
	m_size = size; 
	recalcExtens(); 
}


const std::string& GameObject::getName() const
{ 
	return m_name; 
}


const vec2& GameObject::getPosition() const
{ 
	return m_position; 
} 


float GameObject::getRotation() const
{ 
	return m_rotation;
} 


const vec2& GameObject::getSize() const
{ 
	return m_size;
}


vec2 GameObject::getSizeInTiles() const
{
	vec2 res = getSize();
	res.x *= m_tileScale.x;
	res.y *= m_tileScale.y;
	return res;
}

int GameObject::getType() const 
{
	return m_type;
}


float GameObject::getLeft() const 
{
	return m_topLeft.x;
}


float GameObject::getRight() const
{
	return m_bottomRight.x; 
}


float GameObject::getTop() const
{ 
	return m_topLeft.y;
}


float GameObject::getBottom() const 
{ 
	return m_bottomRight.y;
}
	

bool GameObject::isInside(const vec2& worldPosition)
{
	return m_bottomRight.x	>= worldPosition.x
		&& m_topLeft.x		<= worldPosition.x
		&& m_bottomRight.y	>= worldPosition.y
		&& m_topLeft.y		<= worldPosition.y;
}

void GameObject::setTileSize(const vec2& tileSize )
{
	m_tileScale.x  = 1.0f / tileSize.x;
	m_tileScale.y  = 1.0f / tileSize.y;
	recalcExtens();
}



bool GameObject::collidesTo( GameObject* other, vec2* collisionNormal )
{
	assert( other != 0 );
	vec2 d1 = other->m_topLeft - m_bottomRight;
	vec2 d2 = m_topLeft - other->m_bottomRight;

	if( d1.x > 0.0f || d1.y > 0.0f || d2.x > 0.0f || d2.y > 0.0f )
	{
		return false;
	}

	if( collisionNormal != 0 ) 
	{
		collisionNormal->x = d2.x - d1.x;
		collisionNormal->y = d2.y - d1.y;
		collisionNormal->Normalize();
	}

	return true;
}


void GameObject::recalcExtens()
{
	assert(m_size.x >= 0.0f);
	assert(m_size.y >= 0.0f);
	m_topLeft.x		= m_offset.x + m_position.x - (getSizeInTiles().x*0.5f);
	m_topLeft.y		= m_offset.y + m_position.y - (getSizeInTiles().y*0.5f);
	m_bottomRight.x	= m_offset.x + m_position.x + (getSizeInTiles().x*0.5f);
	m_bottomRight.y	= m_offset.y + m_position.y + (getSizeInTiles().y*0.5f);
}



}

