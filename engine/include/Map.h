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
#ifndef MAP_H_
#define MAP_H_


#include <vector>
#include "PropertySet.h"
#include "vec2.h"
#include <Ref.h>

namespace Tmx
{
	class Map;
}

namespace yam2d
{
	
class Camera;
class Tileset;
class Layer;
class Tile;
class GameObject;
class SpriteSheet;

/**
 * Class for Map.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class Map : public Object
{
public:
	typedef std::map<int, Ref<Layer> > LayerMap;

	enum MapOrientation 
	{
		// Indicates, that map is an orthogonal map.
		ORTHOGONAL = 0x01,

		// Indicates, that map is an isometric map.
		ISOMETRIC = 0x02
	};



	/** Layer definitions. */
	enum Layers
	{
		BACKGROUND0 = 0,
		BACKGROUND1,
		BACKGROUND2,
		BACKGROUND3,
		MAPLAYER0,
		MAPLAYER1,
		MAPLAYER2,
		MAPLAYER3,
		MAPLAYER4,
		MAPLAYER5,
		MAPLAYER6,
		MAPLAYER7,
		MAPLAYER8,
		MAPLAYER9,
		MAPLAYER10,
		MAPLAYER11,
		MAPLAYER12,
		MAPLAYER13,
		MAPLAYER14,
		MAPLAYER15,
		MAPLAYER16,
		MAPLAYER17,
		MAPLAYER18,
		MAPLAYER19,
		MAPLAYER20,
		MAPLAYER21,
		MAPLAYER22,
		MAPLAYER23,
		MAPLAYER24,
		MAPLAYER25,
		MAPLAYER26,
		MAPLAYER27,
		MAPLAYER28,
		MAPLAYER29,
		GUILAYER0,
		GUILAYER1,
		GUILAYER2,
		GUILAYER3,
		NUM_LAYERS
	};

	/**
	 * Constructs new Map-object according to parameters.
	 * @param tileWidth			Tile width in pixels.
	 * @param tileHeight		Tile height in pixels.
	 * @param orientation		Orientation of this map, can be some of enum MapOrientation.
	 * @param properties		Properties of this map.
	 */
	Map( float tileWidth, float tileHeight, MapOrientation orientation=ORTHOGONAL, const PropertySet& properties=PropertySet() );
	
	virtual ~Map();

	/**
	 * Renders all visible map layers to the screen.
	 */
	void render();

	/**
	 * Updates all map layers and objects inside layer. Typically this is called once in a frame, before rendering.
	 *
	 * @param deltaTime		Time since last update call, in seconds.
	 */
	virtual void update( float deltaTime );

	/** Converts tile-coodrinate value to screen coordinates. Ignores camera position. */
	vec2 tileToScreenCoordinates(float x, float y);
	vec2 tileToScreenCoordinates(const vec2& pos);

	/** Converts map-coodrinate value to screen coordinates. Takes camera position into account. */
	vec2 mapToScreenCoordinates(float x, float y);
	vec2 mapToScreenCoordinates(const vec2& pos);

	/** Converts tile-coodrinate values to device (OpenGL) coordinates. */
	vec2 tileToDeviceCoordinates(float x, float y);
	vec2 tileToDeviceCoordinates(const vec2& pos);

	/** Converts screen-coodrinate value to tile coordinates. Ignores camera position. */
	vec2 screenToTileCoordinates(float x, float y);
	vec2 screenToTileCoordinates(const vec2& pos);

	/** Converts screen-coodrinate value to map coordinates. Takes camera position into account. */
	vec2 screenToMapCoordinates(float x, float y);
	vec2 screenToMapCoordinates(const vec2& pos);

	/** Converts screen-coodrinate value to device (OpenGL) coordinates. */
	vec2 screenToDeviceCoordinates(float x, float y);
	vec2 screenToDeviceCoordinates(const vec2& pos);

	/** Returns tile width */
	float getTileHeight() const;

	/** Returns tile height */
	float getTileWidth() const;

	/** Returns orientation of this Map. */
	MapOrientation getOrientation() const;

	/** Returns all layers of this map */
	LayerMap& getLayers();

	/** Adds new layer to Map. */
	void addLayer(int index, Layer* layer);

	/** Gets layer by index. */
	Layer* getLayer(int index);

	/** Gets layer by name. */
	Layer* getLayer(const std::string& name);
	
	/** Returns properties of this map */
	PropertySet& getProperties();
	
	/** Returns map main camera */
	Camera* getCamera() const;

	/** Utility function for converting isometric coordinates to orthogonal coordinates */
	static vec2 isometricToOrthogonal(float x, float y);

	/** Utility function for converting orthogonal coordinates to isometric coordinates */
	static vec2 orthogonalToIsometric(float x, float y);

	void deleteGameObject(GameObject* gameObject);

private:
	bool isVisible(GameObject* go,Camera* cam);
	void batchLayer(Layer* layer, bool cullInvisibleObjects);

	Ref<Camera>					m_mainCamera;
protected:	
	MapOrientation				m_orientation;
	float						m_tileWidth;
	float						m_tileHeight;

	void clearMapLayers();
private:

	LayerMap					m_layers;
	PropertySet					m_properties;
	bool						m_needsBatching;
		
	// Hidden
	Map();
	Map(const Map&);
	Map& operator=(const Map&);
};


/**
 * Class for TMX-formatted map.
 */
class TmxMap : public Map
{
public:
	typedef Tileset* (*CreateNewTilesetFuncType)(void* userData, const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties );
	typedef Layer* (*CreateNewLayerFuncType)(void* userData, Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties);
	typedef GameObject* (*CreateNewTileFuncType)(void* userData, Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties);
	typedef GameObject* (*CreateNewGameObjectFuncType)(void* userData, Map* map, Layer* layer, const std::string& type, const vec2& position, const vec2& size, const std::string& name, const PropertySet& properties);

	class MapCreateCallbacks
	{
	public:
		virtual Tileset* createNewTileset( const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties );
		virtual Layer* createNewLayer( Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties);
		virtual GameObject* createNewTile( Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties);
		virtual GameObject* createNewGameObject( Map* map, Layer* layer, const std::string& type, const vec2& position, const vec2& size, const std::string& name, const PropertySet& properties);
	};

	/**
	 * Creates new map.
	 */
	TmxMap();

	virtual ~TmxMap();

	/** Loads map file */
	bool loadMapFile(const std::string& mapFileName);

	/** Returns map width in tiles. */
	float getWidth() const { return m_width; }

	/** Returns map height in tiles. */
	float getHeight() const { return m_height; }

	/** Sets user data, which is passed to registerCreateNewTilesetFunc, registerCreateNewLayerFunc, and registerCreateNewTileFunc when they are called during loadMapFile call */
	void setCallBackData(void* userData) { m_userData = userData; }

	/** 
	 * Registers callback function, which is called each time, when new Tileset is needed to be create during loadMapFile call.
	 */
	void registerCreateNewTilesetFunc( CreateNewTilesetFuncType createNewTileset ) { m_createNewTileset = createNewTileset; }

	/**
	 * Registers callback function, which is called each time, when new Layer is needed to be create during loadMapFile call. 
	 */	
	void registerCreateNewLayerFunc( CreateNewLayerFuncType createNewLayer ) { m_createNewLayer = createNewLayer; }
	
	/**
	 * Registers callback function, which is called each time, when new Tile is needed to be create during loadMapFile call.
	 */
	void registerCreateNewTileFunc( CreateNewTileFuncType createNewTile ) { m_createNewTile = createNewTile; }
	
	/**
	 * Registers callback function, which is called each time, when new Tile is needed to be create during loadMapFile call.
	 */
	void registerCreateNewGameObjectFunc( CreateNewGameObjectFuncType createNewGameObject ) { m_createNewGameObject = createNewGameObject; }

	/**
	 * Registers callback object, which corresponding method is called each time, when new Tileset, Layer orTile is needed to be create during loadMapFile call.
	 */
	void registerMapCreateCallbacks(MapCreateCallbacks* callbacks);
protected:
/*	/** Can be overwritten in derived class for create custom Tilesets. */
//	static Tileset* createNewTileset(void* userData, const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties );

	/** Can be overwritten in derived class for create custom Layers. */
//	static Layer* createNewLayer(void* userData, Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties);

	/** Can be overwritten in derived class for create custom Tiles. */
//	static Tile* createNewTile(void* userData, Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties);

private:
	float						m_width;
	float						m_height;
	void*						m_userData;
	CreateNewTilesetFuncType	m_createNewTileset;
	CreateNewLayerFuncType		m_createNewLayer;
	CreateNewTileFuncType		m_createNewTile;
	CreateNewGameObjectFuncType m_createNewGameObject;
	std::vector< Ref<Tileset> > m_tilesets;

	// Hidden
	TmxMap(const TmxMap&);
	TmxMap& operator=(const TmxMap&);
	
};


}


#endif