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
#include "TileComponent.h"
#include <Tileset.h>
#include "Layer.h"
#include "es_util.h"
#include <tmx-parser/Tmx.h>
#include <Texture.h>
#include <Camera.h>
#include <config.h>
#include <MapController.h>
#include <ElapsedTimer.h>


namespace yam2d
{

using namespace std;

Component* DefaultComponentFactory::createNewComponent(const std::string& type, Entity* owner, const yam2d::PropertySet& properties)
{
	if ("Layer" == type)
	{
		std::string name = properties["name"].get<std::string>();
		float opacity = properties["opacity"].get<float>();
		bool isVisible = properties.getOrDefault<bool>("visible", true);
		bool isStatic = false;
		 
		if (properties.hasProperty("static") )
		{
			std::string s = properties["static"].get<std::string>();
			if ( (s == "true") || (s == "1"))
				isStatic = true;
		}
		Map* map = dynamic_cast<Map*>(owner);
		assert(map != 0);
		return new Layer(map, name, opacity, isVisible, isStatic, properties);
	}
	if ("Tile" == type)
	{
		//	int gameObjectType = 0;
		//vec2 position = vec2(properties["positionX"].get<float>(), properties["positionY"].get<float>());
		unsigned id = properties["id"].get<int>();
		bool flippedHorizontally = properties.getOrDefault("flippedHorizontally", false);
		bool flippedVertically = properties.getOrDefault("flippedVertically", false);
		bool flippedDiagonally = properties.getOrDefault("flippedDiagonally", false);
		TileComponent* tileComponent = new TileComponent(owner, /*position,*/ id, flippedHorizontally, flippedVertically, flippedDiagonally);
		return tileComponent;
	}
	else
	{
		assert(0); // Unknown type
	}

	return 0;
}

Entity* DefaultComponentFactory::createNewEntity(ComponentFactory* componentFactory, const std::string& type, Entity* parent, const yam2d::PropertySet& properties)
{
	GameObject* gameObject = new GameObject(parent, properties);

	(void)componentFactory;
	if ("Tile" == type)
	{
		gameObject->addComponent(createNewComponent(type, gameObject, properties));
	}
	else
	{
		if (type.length() > 0)
		{
			esLogMessage("Warning: Creating game object of unknown type: \"%s\".", type.c_str());
		}
		else
		{
			esLogMessage("Warning: Creating game object of unknown (empty) type.");
		}
	}

	return gameObject;
}

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

	Tileset* defaultCreateNewTileset(void*, const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties )
	{
		return new Tileset(name, spriteSheet, tileOffsetX, tileOffsetY, properties);
	}


	Layer* defaultCreateNewLayer(void*, Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties)
	{
		return new Layer(map, name, opacity, visible, false, properties); //create dynamic layer
	}

	/*
	GameObject* defaultCreateNewTile(void*, Map* map, Layer* , const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& )
	{
		return new Tile(0, position, tileset, id, flippedHorizontally, flippedVertically, flippedDiagonally, map->getTileWidth(), map->getTileHeight());
	}

	GameObject*  defaultCreateNewGameObject(void*, Map* , Layer* , const std::string& , const vec2& position, const vec2& size, const std::string& name, const PropertySet& )
	{
		return new GameObject(0,position,size,name);
	}

	Tileset* createNewTileset_MapCreateCallbacks(void* userData, const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties )
	{
		TmxMap::MapCreateCallbacks* o = static_cast<TmxMap::MapCreateCallbacks*>(userData);
		assert(o != 0);
		return o->createNewTileset(name,spriteSheet,tileOffsetX,tileOffsetY,properties);
	}


	Layer* createNewLayer_MapCreateCallbacks(void* userData, Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties)
	{
		TmxMap::MapCreateCallbacks* o = static_cast<TmxMap::MapCreateCallbacks*>(userData);
		assert(o != 0);
		return o->createNewLayer(map,name,opacity,visible,properties);
	}


	GameObject* createNewTile_MapCreateCallbacks(void* userData, Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties)
	{
		TmxMap::MapCreateCallbacks* o = static_cast<TmxMap::MapCreateCallbacks*>(userData);
		assert(o != 0);
		return o->createNewTile(map,layer,position,tileset,id,flippedHorizontally,flippedVertically,flippedDiagonally,properties);
	}


	GameObject* createNewGameObject_MapCreateCallbacks(void* userData, Map* map, Layer* layer, const std::string& type, const vec2& position, const vec2& size, const std::string& name, const PropertySet& properties)
	{
		TmxMap::MapCreateCallbacks* o = static_cast<TmxMap::MapCreateCallbacks*>(userData);
		assert(o != 0);
		return o->createNewGameObject(map,layer,type, position,size, name, properties);
	}*/


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
	: Entity(0,0,properties)
	, m_mainCamera(new Camera(this))
	, m_orientation( orientation )
	, m_tileWidth( tileWidth )
	, m_tileHeight( tileHeight )
	, m_layers()
	, m_properties(properties)
	, m_needsBatching(true)
{
}

Map::~Map()
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

void Map::deleteGameObject(GameObject* gameObject)
{
	for( int l=0; l<NUM_LAYERS; ++l )
	{
		Layer* layer = m_layers[l];
		if( layer )
		{
			layer->deleteGameObjectIfExist(gameObject);
		}
	}
}

GameObject* Map::findGameObjectByName(const std::string& name)
{
	for( int l=0; l<NUM_LAYERS; ++l )
	{
		Layer* layer = m_layers[l];
		if( layer )
		{
			Layer::GameObjectList& gos = layer->getGameObjects();
			for(size_t i=0; i<gos.size(); ++i )
			{
				if( gos[i]->getName() == name )
				{
					return gos[i];
				}
			}
		}
	}

	return 0;
}

vec2 Map::tileToScreenCoordinates(float x, float y)
{
	if( m_orientation == ISOMETRIC )
	{
		vec2 res = isometricToOrthogonal(x,y);
		res.x *= m_tileWidth;
		res.y *= m_tileHeight;
		return res;
	}
	else if( m_orientation == ORTHOGONAL )
	{
		float tileWidth = float(m_tileWidth);
		float tileHeight = float(m_tileHeight);
		float x_pos = (x * tileWidth );
		float y_pos = (y * tileHeight);
		return vec2(x_pos,y_pos);
	}
	else
	{
		assert(0);
	}

	return vec2(0);
}


vec2 Map::tileToScreenCoordinates(const vec2& pos)
{
	return tileToScreenCoordinates(pos.x,pos.y);
}

/*vec2 Map::mapToScreenCoordinates(float x, float y)
{
	return mapToScreenCoordinates(vec2(x,y));
}

vec2 Map::mapToScreenCoordinates(const vec2& pos)
{
	as
	//return getCamera()->getPosition() + pos;

	//tileToScreenCoordinates();
//	return mapToScreenCoordinates(pos.x,pos.y);	
}
*/

vec2 Map::screenToTileCoordinates(float x, float y)
{
	if( m_orientation == ISOMETRIC )
	{
		vec2 res = isometricToOrthogonal(x,y);
		res.x /= m_tileWidth;
		res.y /= m_tileHeight;
		return res;
	}
	else if( m_orientation == ORTHOGONAL )
	{
		float tileWidth = float(m_tileWidth);
		float tileHeight = float(m_tileHeight);
		float x_pos = (x / tileWidth );
		float y_pos = (y / tileHeight);
		return vec2(x_pos*getCamera()->getScaleFactorX(),y_pos*getCamera()->getScaleFactorY());
	}
	else
	{
		assert(0);
	}

	return vec2(0);
}

vec2 Map::screenToTileCoordinates(const vec2& pos)
{
	return screenToTileCoordinates(pos.x,pos.y);
}

vec2 Map::screenToMapCoordinates(float x, float y)
{
	return screenToMapCoordinates(vec2(x,y));
}

vec2 Map::screenToMapCoordinates(const vec2& pos)
{
	vec2 camHalfSize =  getCamera()->getSize();
	camHalfSize *= 0.5f;
	vec2 posInTile = screenToTileCoordinates(pos);
	vec2 res =  posInTile + getCamera()->getPosition() - camHalfSize;
	return res;
}

vec2 Map::tileToDeviceCoordinates(float x, float y)
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

vec2 Map::tileToDeviceCoordinates(const vec2& pos)
{
	return tileToDeviceCoordinates(pos.x,pos.y);
}

vec2 Map::screenToDeviceCoordinates(float x, float y)
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


vec2 Map::screenToDeviceCoordinates(const vec2& pos)
{
	return screenToDeviceCoordinates(pos.x,pos.y);
}


float Map::getTileHeight() const 
{ 
	return m_tileHeight;
}


float Map::getTileWidth() const
{ 
	return m_tileWidth;
}


Map::MapOrientation Map::getOrientation() const
{ 
	return m_orientation;
}


Map::LayerMap& Map::getLayers()
{
	return m_layers;
}


void Map::addLayer(int index, Layer* layer)
{
	assert( m_layers[index] == 0 ); // Error! There is already layen in given index!
	layer->setLayerIndex(index);
	m_layers[index] = layer;
}


Layer* Map::getLayer(int index)
{
	assert( m_layers[index] != 0 );
	return m_layers[index].ptr();
}

Layer* Map::getLayer(const std::string& name)
{
	// Render visible layers
	for( int i=0; i<NUM_LAYERS; ++i )
	{
		Layer* layer = m_layers[i];

		if( layer && layer->getName() == name )
		{
			return layer;
		}
	}

	return 0;
}

PropertySet& Map::getProperties()
{
	return m_properties; 
}
	

Camera* Map::getCamera() const
{
	return m_mainCamera.ptr();
}


bool Map::isVisible(GameObject* go, Camera* cam)
{
	(void)go;
	(void)cam;
//	return true;
	return cam->collidesTo(go);
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
		//Camera* cam = getCamera();
		for( size_t i=0; i<gameObjects.size(); ++i )
		{
			GameObject* go = gameObjects[i];

			// TODO: Culling does not work??
			//if( isVisible(go,cam) )
			{
				gameObjectsToBeRendered.push_back(go);
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
/*	if( !layer->isStatic() && getOrientation() == ISOMETRIC )
	{
		std::qsort( &gameObjectsToBeRendered[0], gameObjectsToBeRendered.size(), sizeof(GameObject*), compareXY );
		delta = 1.0f/gameObjectsToBeRendered.size(); 
	}*/
	
	// Render all GameObjects.
	for( size_t i=0; i<gameObjectsToBeRendered.size(); ++i )
	{
		layer->setDepth( layer->getDepth()+delta );
		renderLayerObject(gameObjectsToBeRendered[i],layer);
	}	
}


void Map::clearMapLayers()
{
	m_layers.clear();
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
			renderCamera(m_mainCamera, layer);
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
	
	//m_mainCamera->render();
	// Render visible layers
	for( int i=0; i<NUM_LAYERS; ++i )
	{
		Layer* layer = m_layers[i];

		if( layer && layer->isVisible() )
		{
			renderCamera(m_mainCamera,layer);
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

		if( layer && layer->isUpdatable() )
		{
			updateLayer(layer,deltaTime);
		}
	}
	
	// Delete unneeded layer gameobjects
	for( int i=0; i<NUM_LAYERS; ++i )
	{
		Layer* layer = m_layers[i];

		if( layer && !layer->isStatic() )
		{
			layer->deleteUnneededObjects();
		}
	}
}

/*
Tileset* TmxMap::MapCreateCallbacks::createNewTileset( const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties )
{
	return defaultCreateNewTileset(0, name, spriteSheet, tileOffsetX, tileOffsetY, properties);
}

Layer* TmxMap::MapCreateCallbacks::createNewLayer( Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties)
{
	return defaultCreateNewLayer(0, map, name, opacity, visible, properties);
}

GameObject* TmxMap::MapCreateCallbacks::createNewTile( Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties)
{
	return defaultCreateNewTile(0, map, layer, position, tileset, id, flippedHorizontally, flippedVertically, flippedDiagonally, properties);
}

GameObject* TmxMap::MapCreateCallbacks::createNewGameObject( Map* map, Layer* layer, const std::string& type, const vec2& position, const vec2& size, const std::string& name, const PropertySet& properties)
{
	return defaultCreateNewGameObject(0, map, layer, type, position, size, name, properties);
}

*/





bool TmxMap::loadMapFile(const std::string& mapFileName, ComponentFactory* componentFactory)
{

	yam2d::ElapsedTimer timer;
	timer.reset();
	//esLogMessage("Parsing tmx-file");
	m_loadedMapFileName = mapFileName;
	std::string path = getPath(mapFileName);	

	Tmx::Map map;
	map.ParseFile(mapFileName.c_str());
	
	if( map.HasError() )
	{
		esLogEngineError("[%s] Map file: %s could not be found!", __FUNCTION__, mapFileName.c_str() ); 
		return false;
	}

	m_orientation = (map.GetOrientation() == Tmx::TMX_MO_ISOMETRIC) ? ISOMETRIC : ORTHOGONAL;
	m_tileWidth = float(map.GetTileWidth());
	m_tileHeight = float(map.GetTileHeight());
	m_width = float(map.GetWidth());
	m_height = float(map.GetHeight());
	m_tilesets.clear();

	getProperties().setValues(map.GetProperties().GetList());
	//esLogMessage("Parsing tmx-file done. Time: %2.4f", timer.getTime());

	// Create tilesets
	timer.reset();
	//esLogMessage("Creating %d tilesets", (int)m_tilesets.size());
	m_tilesets.resize(map.GetNumTilesets());
	for( size_t i=0; i<m_tilesets.size(); ++i )
	{
		const Tmx::Tileset* tileset = map.GetTileset(i);
		std::string texFileName = path + tileset->GetImage()->GetSource();
		//esLogEngineDebug("Creating tileset: %s from texture \"%s\"", tileset->GetName().c_str(), texFileName.c_str() );
		Texture* texture = new Texture( texFileName.c_str() );
		
		// convert transparent_color
		std::string transparentColor = tileset->GetImage()->GetTransparentColor();
		if( transparentColor.length() > 0 )
		{
			int color(0);
#if defined(_WIN32)
			sscanf_s( transparentColor.c_str(), "%X", &color );
#else
			sscanf( transparentColor.c_str(), "%X", &color );
#endif
			texture->setTransparentColor( (unsigned char)((color&0xff0000) >> 16), (unsigned char)((color&0xff00) >> 8), (unsigned char)((color&0xff) >> 0) );
		}
		
		// Create sprite sheet
		SpriteSheet* spriteSheet = SpriteSheet::generateSpriteSheet(texture, tileset->GetImage()->GetWidth(), tileset->GetImage()->GetHeight(),
			tileset->GetTileWidth(), tileset->GetTileHeight(),
			tileset->GetMargin(), tileset->GetMargin(),
			tileset->GetSpacing(), tileset->GetSpacing() );
		PropertySet properties;
		properties.setValues(tileset->GetProperties().GetList());
		//assert( m_createNewTileset != 0 );
		m_tilesets[i] = defaultCreateNewTileset(0, tileset->GetName(), spriteSheet, float(tileset->GetTileOffsetX()), float(tileset->GetTileOffsetY()), properties);
		assert( m_tilesets[i] != 0 ); // You must return new Tileset in createTileset callback!!
	}

	//esLogMessage("Creating tilesets done. Time: %2.4f", timer.getTime());

	// Create layers

	timer.reset();
	//esLogMessage("Creating %d layers", map.GetNumLayers());
	for( int i=0; i<map.GetNumLayers(); ++i )
	{
		const Tmx::Layer* l = map.GetLayer(i);
		PropertySet properties;
		
		properties.setValues(l->GetProperties().GetList());
		//esLogEngineDebug("Creating layer # MAPLAYER%d : \"%s\" visible: %s", i, l->GetName().c_str(), l->IsVisible() ? "true":"false" );

		properties["type"] = "Layer";
		properties["name"] = l->GetName();
		properties["layerIndex"] = (int)MAPLAYER0 + i;
		properties["opacity"] = l->GetOpacity();
		properties["visible"] = l->IsVisible();
		addLayer(MAPLAYER0 + i, (Layer*)componentFactory->createNewComponent("Layer", this, properties));
		assert(getLayers()[MAPLAYER0+i] != 0); // You must return new Layer in createLayer callback!!
	}
	//esLogMessage("Creating layers done. Time: %2.4f", timer.getTime());

	// Create tiles
	for( int i=0; i<map.GetNumLayers(); ++i )
	{
		if( dynamic_cast<const Tmx::TileLayer*>(map.GetLayer(i)) )
		{
			int numObjects = 0;
			const Tmx::TileLayer* const l = dynamic_cast<const Tmx::TileLayer*>(map.GetLayer(i));
			float timeProperties = 0.0f;
			float timeCreate = 0.0f;
			
			getLayers()[MAPLAYER0 + i]->reserve(l->GetHeight()*l->GetWidth());

			//esLogMessage("Creating %d tile layer tiles for layer", l->GetHeight()*l->GetWidth());

			for( int y=0; y<l->GetHeight(); ++y )
			{
				for( int x=0; x<l->GetWidth(); ++x )
				{
					timer.reset();
					const Tmx::MapTile& tile = l->GetTile(x,y);
					
					if (tile.tilesetId >= 0 && tile.tilesetId < map.GetNumTilesets())
					{
						Tileset* tileset = m_tilesets[tile.tilesetId];
						const Tmx::Tileset* ts = map.GetTileset(tile.tilesetId);
						const Tmx::Tile* tileSetTile = ts->GetTile(tile.id);
						PropertySet properties;
						if (tileSetTile != 0)
						{
							properties.setValues(tileSetTile->GetProperties().GetList());
						}
					
						vec2 sizeInTiles(float(ts->GetTileWidth()) / float(getTileWidth()), float(ts->GetTileHeight()) / float(getTileHeight()));
						if (!properties.hasProperty("type"))
						{
							properties["type"] = "Tile";
						}
						properties["positionX"] = (float)x - 1.0f + 0.5f*sizeInTiles.x;
						properties["positionY"] = (float)y;
						properties["sizeX"] = sizeInTiles.x;
						properties["sizeY"] = sizeInTiles.y;
						properties["id"] = (int)tile.id;

						if (tile.flippedHorizontally)
							properties["flippedHorizontally"] = tile.flippedHorizontally;

						if (tile.flippedVertically)
							properties["flippedVertically"] = tile.flippedVertically;

						if (tile.flippedDiagonally)
							properties["flippedDiagonally"] = tile.flippedDiagonally;

						timeProperties += timer.getTime();
						timer.reset();
						GameObject* gameObject = (GameObject*)componentFactory->createNewEntity(componentFactory, properties["type"].get<std::string>(), getLayers()[MAPLAYER0 + i], properties);
						if (gameObject != 0)
						{
							gameObject->getComponent<TileComponent>()->setTileSet(tileset);
							++numObjects;
							getLayers()[MAPLAYER0 + i]->addGameObject(gameObject);
						}
						timeCreate += timer.getTime();
					}
				}
			}

			//esLogMessage("numObjects: %d timeCreate: %2.3f, timeProperties: %2.3f objs/s: %4.2f", numObjects, timeCreate, timeProperties,
			//	(float)numObjects / (timeCreate + timeProperties));

			//esLogEngineDebug("Created %d objects to tile layer \"%s\"", numObjects, l->GetName().c_str() );

			//esLogMessage("Creating tile layer tiles done.");
		}
		else
		{
			//esLogMessage("Creating object layer objects");
			int numObjects = 0;
			const Tmx::ObjectLayer* const l = dynamic_cast<const Tmx::ObjectLayer*>(map.GetLayer(i));
			assert(l);
			const std::vector< Tmx::Object* > objects = l->GetObjects();

			for (size_t x = 0; x < objects.size(); ++x)
			{
				const Tmx::Object* o = objects[x];
				GameObject* tNew = 0;

				// Tiled has different coordinates for objects. In Tiled the tile object has zero in bottom left corner. Adjust accordingly. Also the rotation is reverse and in degrees.
				vec2 positionInTiles(float(o->GetX()) / float(getTileWidth()), float(o->GetY()) / float(getTileHeight()));
				vec2 sizeInTiles(float(o->GetWidth()) / float(getTileWidth()), float(o->GetHeight()) / float(getTileHeight()));

				// COnvert coordinates to yam2d.
				positionInTiles.x = positionInTiles.x + sizeInTiles.x * 0.5f - 1.0f;
				positionInTiles.y = positionInTiles.y + sizeInTiles.y * 0.5f - 0.5f;

				PropertySet properties;
				properties.setValues(o->GetProperties().GetList());

				if (o->GetType().length() > 0)
				{
					properties["type"] = o->GetType();
				}
				properties["name"] = o->GetName();
				properties["positionX"] = positionInTiles.x;
				properties["positionY"] = positionInTiles.y;
				properties["sizeX"] = sizeInTiles.x;
				properties["sizeY"] = sizeInTiles.y;
	
				if( o->GetGid() > 0 )
				{
					int tilesetIndex = map.FindTilesetIndex(o->GetGid());

					Tileset* tileset = m_tilesets[tilesetIndex];
					const Tmx::Tileset* ts = map.GetTileset(tilesetIndex);

					int id = o->GetGid() - ts->GetFirstGid();
					const Tmx::Tile* tileSetTile = ts->GetTile(id);

					properties["id"] = id;

					if (tileSetTile != 0)
					{
						std::map< std::string, std::string > v = tileSetTile->GetProperties().GetList();
						for (std::map< std::string, std::string >::const_iterator it = v.begin(); it != v.end(); ++it)
						{
							properties[it->first] = it->second;
						}
					}

					if (!properties.hasProperty("type"))
					{
						std::string type = o->GetType();
						if (type == "")
						{
							type = "Tile";
						}
						
						properties["type"] = type;
					}


					properties["positionY"] = positionInTiles.y - 1.0f;
					properties["positionX"] = positionInTiles.x;// -0.5f*sizeInTiles.x;
					
					tNew = (GameObject*)componentFactory->createNewEntity(componentFactory, properties["type"].get<std::string>(), getLayers()[MAPLAYER0 + i], properties);
					if (tNew != 0)
					{
						assert(tNew->getComponent<TileComponent>() != 0); // You need to add Tile component to the game object.
						tNew->getComponent<TileComponent>()->setTileSet(tileset);
					}
				}
				else if( o->GetPolygon() != 0)
				{
					esLogEngineError("Polygons not yet implemented in map load");
					// polygon
				/*	size
					position
					o->GetPolygon() 
					o->GetName();
					o->GetProperties();
					o->GetType();*/
				}
				else if( o->GetPolyline() != 0 )
				{
					esLogEngineError("Polylines not yet implemented in map load");
					/*size
					position
					o->GetPolyline() 
					o->GetName();
					o->GetProperties();
					o->GetType();*/
				}
				else if( o->isEllipse() )
				{
					esLogEngineError("Ellipses not yet implemented in map load");
					// ellipse
				/*	size
					position
					o->GetPolyline() 
					o->GetName();
					o->GetProperties();
					o->GetType();*/
				}
				else
				{
					// regular game object
					tNew = (GameObject*)componentFactory->createNewEntity(componentFactory, o->GetType(), getLayers()[MAPLAYER0 + i], properties);
				}

				if( tNew != 0 )
				{
					++numObjects;
					tNew->setTileSize(vec2(m_tileWidth, m_tileHeight));
					tNew->setSize(vec2(float(o->GetWidth()), float(o->GetHeight())));
					getLayers()[MAPLAYER0+i]->addGameObject(tNew);
				}
			}

		//	esLogEngineDebug("Created %d objects to object layer \"%s\"", numObjects, l->GetName().c_str());
		//	esLogMessage("Creating object layer objects done.");
		}		
	}	
	
	return true;
}

TmxMap::TmxMap()
	: Map( 0, 0, ORTHOGONAL )
	, m_width( 0 )
	, m_height( 0 )
	/*, m_userData( 0 )
	, m_createNewTileset( defaultCreateNewTileset )
	, m_createNewLayer( defaultCreateNewLayer )
	, m_createNewTile( defaultCreateNewTile )
	, m_createNewGameObject( defaultCreateNewGameObject )*/
	, m_tilesets()
	, m_loadedMapFileName("")
{
}


TmxMap::~TmxMap()
{
}
/*
void TmxMap::registerMapCreateCallbacks(MapCreateCallbacks* callbacks)
{
	setCallBackData(callbacks);
	registerCreateNewTilesetFunc( createNewTileset_MapCreateCallbacks );
	registerCreateNewLayerFunc( createNewLayer_MapCreateCallbacks );
	registerCreateNewTileFunc( createNewTile_MapCreateCallbacks );
	registerCreateNewGameObjectFunc( createNewGameObject_MapCreateCallbacks );
}*/


}

