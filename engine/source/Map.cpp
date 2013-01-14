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

#include "Map.h"
#include "Tile.h"
#include "Layer.h"
#include "es_util.h"
#include <tmx-parser/Tmx.h>

namespace yam2d
{

using namespace std;


// anonymous namespace for internal functions
namespace
{	
	int compareXY(const void* o1, const void* o2)
	{
		GameObject* go1 = *((GameObject**)o1);
		GameObject* go2 = *((GameObject**)o2);

		vec2 p1 = Map::isometricToOrthogonal(go1->getLeft(),go1->getTop());
		vec2 p2 = Map::isometricToOrthogonal(go2->getLeft(),go2->getTop());

		return int((p1.y-p2.y)*2000.0f);
	}

	/*
	bool compareXY(GameObject* go1, GameObject* go2 )
	{
		vec2 p1 = go1->getPosition();
		vec2 p2 = go2->getPosition();

		float min1 = p1.x < p1.y ? p1.x : p1.y;
		float min2 = p2.x < p2.y ? p2.x : p2.y;
		
		if( min1 - min2 == 0 )
		{
			float len1 = p1.x*p1.x + p1.y*p1.y;
			float len2 = p2.x*p2.x + p2.y*p2.y;
		//	return len1>len2;
		}

		return min1<min2;
	}*/
}


Map::Map( float tileWidth, float tileHeight, MapOrientation orientation, const PropertySet& properties )
	: m_mainCamera(new Camera(0))
	, m_orientation( orientation )
	, m_tileWidth( tileWidth )
	, m_tileHeight( tileHeight )
	, m_layers()
	, m_properties()
	, m_needsBatching(true)
{
}


vec2 Map::isometricToOrthogonal(float x, float y)
{
	float x_pos = (x * 0.5f ) - (y * 0.5f);
	float y_pos = ( (x * 0.5f) + (y * 0.5f));
	return vec2(x_pos,y_pos);
}


vec2 Map::orthogonalToIsometric(float x, float y)
{
	x *= 2.0f;
	y *= 2.0f;

	float y1 = (y-x) * 0.5f;
	float x1 = y-y1;
	return vec2(x1,y1);
}


vec2 Map::tileToScreenCoordinates(float x, float y)
{
	if( m_orientation == ISOMETRIC )
	{
		vec2 res = isometricToOrthogonal(x,y);
		res.x *= m_tileWidth;
		res.y *= -m_tileHeight;
		return res;
	}
	else if( m_orientation == ORTHOGONAL )
	{
		float tileWidth = float(m_tileWidth);
		float tileHeight = float(m_tileHeight);
		float x_pos = (x * tileWidth );
		float y_pos = -(y * tileHeight);
		return vec2(x_pos,y_pos);
	}
	else
	{
		assert(0);
	}

	return vec2(0);
}


vec2 Map::screenToTileCoordinates(float x, float y)
{
	if( m_orientation == ISOMETRIC )
	{
		y *= -1.0f;
		x /= m_tileWidth;
		y /= m_tileHeight;
		return orthogonalToIsometric(x,y);
	}
	else if( m_orientation == ORTHOGONAL )
	{
		y *= -1.0f;
		x /= m_tileWidth;
		y /= m_tileHeight;
		return vec2(x,y);
	}
	else
	{
		assert(0);
	}

	return vec2(0);
}


bool Map::isVisible(GameObject* go, Camera* cam)
{
	float left		= go->getLeft();
	float top		= go->getTop();
	float right		= go->getRight();
	float bottom	= go->getBottom();
		
	float camLeft	= cam->getLeft();
	float camTop	= cam->getTop();
	float camRight	= cam->getRight();
	float camBottom	= cam->getBottom();
	
	if( m_orientation == ISOMETRIC )
	{
		vec2 camPos = cam->getPosition();
		vec2 v = isometricToOrthogonal(left,top);
		left	= v.x;
		top		= v.y;
		
		v = isometricToOrthogonal(right,bottom);
		right	= v.x;
		bottom	= v.y;
		
		camPos		= isometricToOrthogonal(camPos.x,camPos.y);
		camLeft		= camPos.x - cam->getSize().x*0.5f;
		camTop		= camPos.y - cam->getSize().y*0.5f;
		camRight	= camPos.x + cam->getSize().x*0.5f;
		camBottom	= camPos.y + cam->getSize().y*0.5f;
	}

	if( (camTop < bottom) && (camBottom >= top) && (camRight >= left) && (camLeft < right) )
	{
		return true;
	}
	
	return false;
}


void Map::batchLayer(Layer* layer, bool cullInvisibleObjects)
{
	assert( layer->isVisible() );

	// Clear batch
	layer->getBatch()->clear();
	
	Layer::GameObjectList& gameObjects = layer->getGameObjects();

	// Make render list
	std::vector<GameObject*> gameObjectsToBeRendered;

	if( cullInvisibleObjects )
	{
		Camera* cam = getCamera();
		for( size_t i=0; i<gameObjects.size(); ++i )
		{
			GameObject* go = gameObjects[i];

			if( isVisible(go,cam) )
			{
				gameObjectsToBeRendered.push_back(gameObjects[i]);
			}
		}
	}
	else
	{
		for( size_t i=0; i<gameObjects.size(); ++i )
		{
			gameObjectsToBeRendered.push_back(gameObjects[i]);
		}
	}

	// Don't render of no objects.
	if( gameObjectsToBeRendered.size() <= 0 )
	{
		return;
	}

	float delta = 0.0f;
	
	// Sort isometric dynamic layers.
	// TODO: There is some bug and sorting does not work correctly, mainly due GameObject sizes are incorrect etc.
	if( !layer->isStatic() && getOrientation() == ISOMETRIC )
	{
		std::qsort( &gameObjectsToBeRendered[0], gameObjectsToBeRendered.size(), sizeof(GameObject*), compareXY );
		delta = 1.0f/gameObjectsToBeRendered.size(); 
	}
	
	// Render all GameObjects.
	for( size_t i=0; i<gameObjectsToBeRendered.size(); ++i )
	{
		layer->setDepth( layer->getDepth()+delta );
		gameObjectsToBeRendered[i]->render(layer);
	}	
}


void Map::render()
{
	// Batch static layers
	if( m_needsBatching )
	{
		for( int i=0; i<NUM_LAYERS; ++i )
		{
			Layer* layer = m_layers[i];
			if( layer && layer->isVisible() && layer->isStatic() )
			{
				m_layers[i]->setDepth( float(i) );
				batchLayer(m_layers[i],false);
			}
		}

		m_needsBatching = false;
	}

	// Batch dynamic layers
	for( int i=0; i<NUM_LAYERS; ++i )
	{
		Layer* layer = m_layers[i];
		if( layer && layer->isVisible() && !layer->isStatic() )
		{
			m_layers[i]->setDepth( float(i) );
			m_mainCamera->render(layer);

			if( i >= MAPLAYER0 && i<= MAPLAYER9 )
			{
				batchLayer(m_layers[i],true);
			}
			else
			{				
				batchLayer(m_layers[i],false);
			}
		}
	}
		
	// Render visible layers
	for( int i=0; i<NUM_LAYERS; ++i )
	{
		Layer* layer = m_layers[i];

		if( layer && layer->isVisible() )
		{
			m_mainCamera->render(layer);
			layer->getBatch()->render();
		}
	}
}


void Map::update( float deltaTime )
{
	// Update all layers
	for( int i=0; i<NUM_LAYERS; ++i )
	{
		Layer* layer = m_layers[i];

		if( layer && !layer->isStatic() )
		{
			layer->update(deltaTime);
		}
	}
}


TmxMap::TmxMap(Tmx::Map* map)
	: Map( float(map->GetTileWidth()), float(map->GetTileHeight()), (map->GetOrientation() == Tmx::TMX_MO_ISOMETRIC) ? ISOMETRIC : ORTHOGONAL )
	, m_width( float(map->GetWidth()) )
	, m_height( float(map->GetHeight()) )
	, m_tilesets()
{	
	getProperties().setValues(map->GetProperties().GetList());

	// Create tilesets
	m_tilesets.resize(map->GetNumTilesets());
	for( size_t i=0; i<m_tilesets.size(); ++i )
	{
		const Tmx::Tileset* tileset = map->GetTileset(i);
		esLogMessage("Creating tileset: %s from texture \"%s\"", tileset->GetName().c_str(), tileset->GetImage()->GetSource().c_str() );
		Texture* texture = new Texture( tileset->GetImage()->GetSource().c_str() );
		
		// convert transparent_color
		std::string transparentColor = tileset->GetImage()->GetTransparentColor();
		if( transparentColor.length() > 0 )
		{
			int color(0);
			sscanf_s( transparentColor.c_str(), "%X", &color );
			texture->setTransparentColor(((color&0xff0000) >> 16), ((color&0xff00) >> 8),((color&0xff) >> 0) );
		}
		
		// Create sprite sheet
		SpriteSheet* spriteSheet = SpriteSheet::generateSpriteSheet(texture, tileset->GetTileWidth(), tileset->GetTileHeight(), tileset->GetMargin(), tileset->GetSpacing() );
		PropertySet properties;
		properties.setValues(tileset->GetProperties().GetList());
		m_tilesets[i] = createNewTileset(tileset->GetName(), spriteSheet, float(tileset->GetTileOffsetX()) , float(tileset->GetTileOffsetY()),properties);
		assert( m_tilesets[i] != 0);
	}

	// Create layers
	for( int i=0; i<map->GetNumLayers(); ++i )
	{
		const Tmx::Layer* l = map->GetLayer(i);
		PropertySet properties;
		properties.setValues(l->GetProperties().GetList());
		esLogMessage("Creating layer # MAPLAYER%d : \"%s\" visible: %s", i, l->GetName().c_str(), l->IsVisible() ? "true":"false" );
		getLayers()[MAPLAYER0+i] = createNewLayer( this, l->GetName(), l->GetOpacity(), l->IsVisible(), properties );
		assert(getLayers()[MAPLAYER0+i] != 0);
	}

	// Create tiles
	for( int i=0; i<map->GetNumLayers(); ++i )
	{
		const Tmx::Layer* l = map->GetLayer(i);
		int numObjects = 0;
		for( int y=0; y<l->GetHeight(); ++y )
		{
			for( int x=0; x<l->GetWidth(); ++x )
			{
				const Tmx::MapTile t = l->GetTile(x,y);
				PropertySet properties;
				Tileset* tileset = 0;
				if(t.tilesetId >= 0 && t.tilesetId < map->GetNumTilesets() )
				{
					tileset = m_tilesets[t.tilesetId];
					const Tmx::Tileset* ts = map->GetTileset(t.tilesetId);
					const Tmx::Tile* tile = ts->GetTile(t.id);
					if( tile != 0 )
					{
						properties.setValues( tile->GetProperties().GetList() );
					}
				}
				
				if( tileset != 0 )
				{
					++numObjects;
					Tile* tNew = createNewTile( this, getLayers()[MAPLAYER0+i], vec2(float(x),float(y)), tileset, t.id, t.flippedHorizontally, t.flippedVertically, t.flippedDiagonally, properties );
					if( tNew != 0 )
					{
						getLayers()[MAPLAYER0+i]->addGameObject(tNew);
					}
				}
			}
		}

		esLogMessage("Created %d objects to layer \"%s\"", numObjects, l->GetName().c_str() );
	}
}

TmxMap* TmxMap::loadFromMapFile(const std::string& mapFileName)
{
	Tmx::Map map;
	map.ParseFile(mapFileName.c_str());
	return new TmxMap(&map);
}


Tileset* TmxMap::createNewTileset(const std::string name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties )
{
	return new Tileset(name,spriteSheet,tileOffsetX,tileOffsetY,properties);
}


Layer* TmxMap::createNewLayer(Map* map, std::string name, float opacity, bool visible, const PropertySet& properties)
{
	return new Layer(map,name,opacity,visible,false,properties); //create dynamic layer
}


Tile* TmxMap::createNewTile(Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties)
{
	return new Tile(TileGameObjectType, properties, position, tileset, id, flippedHorizontally, flippedVertically, flippedDiagonally, map->getTileWidth(),map->getTileHeight());
}

}

