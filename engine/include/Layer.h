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
#ifndef LAYER_H_
#define LAYER_H_

#include <string>
#include <vector>
#include "PropertySet.h"
#include "SpriteBatch.h"
#include <Ref.h>
#include <Entity.h>

namespace yam2d
{

class Map;
class GameObject;

/**
 * Class for single layer of Map.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com)
 */ 
class Layer : public Entity
{
public:
	typedef std::vector< Ref<GameObject> > GameObjectList;

	/**
	 * Constructs new Layer-object according to parameters.
	 *
	 * @param map				Map, where this layer is attached.
	 * @param name				Human readable name for this layer.
	 * @param opacity			Opacity of this layer, which is used in rendering.
	 * @param visible			Is this map visible on screen.
	 * @param isStaticLayer		Is this static layer. Static layer is layer, which is batched only once. Static layers are faster, but there is limitation, that objects can not be moved afterwards.
	 * @param properties		Properties of this layer.
	 */
	Layer(Map* map, std::string name, float opacity, bool visible, bool isStaticLayer, const PropertySet& properties=PropertySet() );
	
	virtual ~Layer() {}

	/** Adds given GameObject to this layer. */
	void addGameObject(GameObject* gameObject);

	/** Removes given GameObject from this layer. Note GameObjects from static layer can not be removed */
	void deleteGameObject(GameObject* gameObject);
	
	void deleteGameObjectIfExist(GameObject* gameObject);

	/** Returns all GameObjects from this Layer. */
	GameObjectList& getGameObjects();
		
	/** Returns batch of this Layer. */
	SpriteBatchGroup* getBatch();

	/** Sets depth value to be used for this layer, when rendering. Typically this method is not needed to be called by game developer. */
	void setDepth(float depth);
	
	/** Updates all layer GameObjects. Typically this method is not needed to be called by game developer. */
	void update(float deltaTime);

	/** 
	 * Deletes all game objects, which has added to list using deleteGameObject. 
	 * Game objects are deleted on map update, after all objects has uptaded. This "delayed destruction"
	 * makes possible of suicide of game objects. Typically this method is not needed to be called by game developer.
	 */
	void deleteUnneededObjects();

	void setLayerIndex(int layerIndex ) { m_layerNumber = layerIndex; }

	/** Returns name of this Layer. */
	const std::string& getName() const;

	/** Returns true, if this layer is visible. i.e. needed to be rendered. Typically this method is not needed to be called by game developer. */
	bool isVisible() const;

	void setVisible(bool visible);
		
	/** Returns map of this Layer. */
	Map* getMap() const;

	/** Returns opacity of this layer */
	float getOpacity() const;


	void setOpacity(float v);

	/** Returns depth value of this layer */
	float getDepth() const;

	/** Returns true, if this Layer is static. */
	bool isStatic() const;
	
	bool isUpdatable() const { return m_isUpdatable; }
	void disableUpdate() { m_isUpdatable = false; }
	int getLayerIndex() const { return m_layerNumber; }

	GameObject* pick(const vec2& pos) const;

	void reserve(size_t s) { m_gameObjects.reserve(s);  }
private:
	//Map*							m_map;
	std::string						m_name;
	bool							m_visible;
	GameObjectList					m_gameObjects;
	float							m_opacity;
	Ref<SpriteBatchGroup>			m_batch;
	float							m_depthValue;
	bool							m_static;
	bool m_isUpdatable;
	int m_layerNumber;
	GameObjectList					m_objectsToDelete;

	// Hidden
	Layer();
	Layer(const Layer&);
	Layer& operator=(const Layer&);
};



}

#endif
