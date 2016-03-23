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

#include "Layer.h"
#include "es_assert.h"
#include "GameObject.h"
#include <config.h>
#include <Map.h>

namespace yam2d
{

using namespace std;

Layer::Layer(Map* map, std::string name, float opacity, bool visible, bool isStaticLayer, const PropertySet& properties )
	: Entity(map, 0, properties)
//, m_map(map)
, m_name(name)
, m_visible(visible)
, m_gameObjects()
, m_batch( new SpriteBatchGroup() )
, m_opacity(opacity)
, m_static(isStaticLayer)
, m_isUpdatable(true)
, m_layerNumber(-1)
{
}


void Layer::addGameObject(GameObject* gameObject)
{
	assert( gameObject != 0 );
	gameObject->setTileSize(vec2(getMap()->getTileHeight(), getMap()->getTileWidth()));
	m_gameObjects.push_back(gameObject);
	//esLogEngineDebug("Added GameObject: %s to layer: %s", gameObject->getName().c_str(), getName().c_str());
}

void Layer::deleteGameObject(GameObject* gameObject)
{
	assert( !isStatic() ); // Can not remove objects from static layer.
	assert( gameObject != 0 );
	for( size_t i=0; i<m_gameObjects.size(); ++i )
	{
		if( m_gameObjects[i].ptr() == gameObject )
		{
			m_objectsToDelete.push_back(gameObject);
			return;
		}
	}

	assert(0); // Game object not found!!
}

void Layer::deleteGameObjectIfExist(GameObject* gameObject)
{	
	assert( gameObject != 0 );
	for( size_t i=0; i<m_gameObjects.size(); ++i )
	{
		if( m_gameObjects[i].ptr() == gameObject )
		{
			assert( !isStatic() ); // Can not remove objects from static layer.
			m_objectsToDelete.push_back(gameObject);
			return;
		}
	}
}

Layer::GameObjectList& Layer::getGameObjects() 
{
	return m_gameObjects; 
}


SpriteBatchGroup* Layer::getBatch() 
{
	return m_batch; 
}


void Layer::setDepth(float depth) 
{
	m_depthValue = depth;
}

/*
void Layer::update(float deltaTime)
{
	for( size_t i=0; i<m_gameObjects.size(); ++i )
	{
		m_gameObjects[i]->update(deltaTime);
	}
}*/

void Layer::deleteUnneededObjects()
{
	for( size_t i=0; i<m_objectsToDelete.size(); ++i )
	{
		for( size_t j=0; j<m_gameObjects.size(); ++j )
		{
			if(m_objectsToDelete[i] == m_gameObjects[j] )
			{
				m_gameObjects.erase(m_gameObjects.begin()+j);
			}
		}

		esLogEngineDebug("Deleting game object: %s from Layer: %s", m_objectsToDelete[i]->getName().c_str(), getName().c_str() );
		m_objectsToDelete[i] = 0; // Actual call to destructor.
	}

	m_objectsToDelete.clear();
}

const std::string& Layer::getName() const
{
	return m_name; 
}


bool Layer::isVisible() const 
{
	return m_visible;
}

void Layer::setVisible(bool visible)
{
	m_visible = visible;
}

	
Map* Layer::getMap() const 
{ 
	return (Map*)getParent();
}


float Layer::getOpacity() const 
{ 
	return m_opacity; 
}


void Layer::setOpacity(float v)
{
	m_opacity = v;
}


float Layer::getDepth() const
{ 
	return m_depthValue; 
}


bool Layer::isStatic() const
{
	return m_static;
}


GameObject* Layer::pick(const vec2& pos) const
{
	for (size_t i = 0; i < m_gameObjects.size(); ++i)
	{
		if ( m_gameObjects[i]->isInside(pos) )
			return m_gameObjects[i].ptr();
	}

	return 0;
}

}

