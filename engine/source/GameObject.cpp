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

GameObject::GameObject(Entity* parent, const PropertySet& properties)
: Entity(parent, 0, properties)
, m_name(properties.getOrDefault<std::string>("name", "") )
, m_position(vec2(properties.getOrDefault("positionX", 0.0f), properties.getOrDefault("positionY", 0.0f) ))
//, m_offset(vec2(properties["offsetX"].get<float>(), properties["offsetY"].get<float>()))
, m_topLeft(0.0f)
, m_bottomRight(0.0f)
, m_rotation(properties.getOrDefault("rotation", 0.0f))
, m_size(vec2(properties.getOrDefault("sizeX", 0.0f), properties.getOrDefault("sizeY", 0.0f)) )
, m_tileScale(1.0f)
{
	recalcExtens();
}


GameObject::GameObject(Entity* parent, int type, const vec2& position, const vec2& size, const std::string& name)
: Entity( parent, 0, Entity::getDefaultProperties() )
, m_name(name)
, m_position(position)
//, m_offset(0.0f)
, m_topLeft(0.0f)
, m_bottomRight(0.0f)
, m_rotation(0.0f)
, m_size(size)
, m_tileScale(1.0f)
{
	recalcExtens();
	(void)type; // Not needed. TODO: Remove someday
}


GameObject::~GameObject()
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
	static const float PI = 3.14159265f;
	while( rotation > PI )
	{
		rotation = rotation - 2.0f*PI;
	}

	while( rotation < -PI )
	{
		rotation = rotation + 2.0f*PI;
	}

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
/*
int GameObject::getType() const 
{
	return m_type;
}*/


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
	

bool GameObject::isInside(const vec2& worldPosition) const
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



bool GameObject::collidesTo( GameObject* other, vec2* collisionNormalLikeVector )
{
	assert( other != 0 );

	vec2 d1 = other->m_topLeft - m_bottomRight;
	vec2 d2 = m_topLeft - other->m_bottomRight;

	if( d1.x > 0.0f || d1.y > 0.0f || d2.x > 0.0f || d2.y > 0.0f )
	{
		return false;
	}

	if (collisionNormalLikeVector != 0)
	{
		vec2 sthis = getSizeInTiles();
		vec2 so = other->getSizeInTiles();
		vec2 d = d2 - d1;
		d.x /= sthis.x + so.x;
		d.y /= sthis.y + so.y;
		
		*collisionNormalLikeVector = d;
	}

	return true;
}


void GameObject::recalcExtens()
{
	assert(m_size.x >= 0.0f);
	assert(m_size.y >= 0.0f);
	vec2 halfSizeInTiles = getSizeInTiles();
	m_topLeft = m_position - halfSizeInTiles;
	m_bottomRight = m_position + halfSizeInTiles;
	//m_topLeft.x = /*m_offset.x +*/ m_position.x - (sizeInTiles.x*0.5f);
	//m_topLeft.y = /*m_offset.y +*/ m_position.y - (sizeInTiles.y*0.5f);
	//m_bottomRight.x = /*m_offset.x +*/ m_position.x + (sizeInTiles.x*0.5f);
	//m_bottomRight.y = /*m_offset.y +*/ m_position.y + (sizeInTiles.y*0.5f);
	assert(m_topLeft.x <= m_bottomRight.x);
	assert(m_topLeft.y <= m_bottomRight.y);
}



}

