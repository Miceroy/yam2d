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


GameObject::GameObject(int type)
: m_name()
, m_position(0.0f)
, m_offset(0.0f)
, m_topLeft(0.0f)
, m_bottomRight(0.0f)
, m_rotation(0.0f)
, m_size(0.0f)
, m_type(type)
{
	recalcExtens();
}


GameObject::~GameObject()
{
}


void GameObject::update( float deltaTime )
{
}

	
void GameObject::render( Layer* layer )
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


void GameObject::setOffset( const vec2& offset ) 
{ 
	m_offset = offset;
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
	return getRight()	>= worldPosition.x
		&& getLeft()	<= worldPosition.x
		&& getBottom()	>= worldPosition.y
		&& getTop()		<= worldPosition.y;
}


void GameObject::recalcExtens()
{
	assert(m_size.x >= 0.0f);
	assert(m_size.y >= 0.0f);
	m_topLeft.x		= m_position.x - (m_size.x*0.5f) + m_offset.x;
	m_topLeft.y		= m_position.y - (m_size.y*0.5f) + m_offset.y;
	m_bottomRight.x	= m_position.x + (m_size.x*0.5f) + m_offset.x;
	m_bottomRight.y	= m_position.y + (m_size.y*0.5f) + m_offset.y;
}



}

