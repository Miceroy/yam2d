// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2015 Mikko Romppainen
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
#include "Entity.h"


#include <es_util.h>
#include <PropertySet.h>


#if !defined (LOG)
#define LOG yam2d::esLogMessage
#endif

Component* ComponentFactory::createNewComponent( const std::string&, Entity* owner, const yam2d::PropertySet& properties )
{
	return new Component(owner,properties);
}
	
	
Entity* ComponentFactory::createNewEntity( ComponentFactory* componentFactory, const std::string&, Entity* owner, const yam2d::PropertySet& properties )
{
	return new Entity(owner, componentFactory, properties);
}


Component::Component(Entity* owner, const yam2d::PropertySet& properties)
    : Object()
    , m_modified(false)
    , m_owner(owner)
    , m_properties(properties)
{
}



Component::~Component()
{
}


Entity* Component::getOwner()
{
    return m_owner;
}


const Entity* Component::getOwner() const
{
    return m_owner;
}


yam2d::PropertySet Component::getProperties() const
{
    return m_properties;
}


yam2d::PropertySet Component::getDefaultProperties()
{
    static yam2d::PropertySet properties;
    properties["type"] = "Component";
    return properties;
}


void Component::setType(const std::string& type)
{
    setModified(true);
    yam2d::PropertySet& properties = getPropertiesRef();
    properties["type"] = type;
}


std::string Component::getType() const
{
    return getProperties()["type"].get<std::string>();
}


void Component::setAllProperties(const yam2d::PropertySet& properties)
{
    m_properties = properties;
}






// Entity

Entity::Entity(Entity* owner, ComponentFactory* componentFactory, const yam2d::PropertySet& properties)
    : Component(owner,properties)
    , m_components()
{
    setAllProperties(componentFactory,properties);
}


Entity::~Entity()
{
}


yam2d::PropertySet Entity::getProperties() const
{
    yam2d::PropertySet properties = Component::getProperties();

	{ // Set component properties
		yam2d::PropertySet components;
		for( size_t i=0; i<m_components.data().size(); ++i )
		{
			const Component* component = m_components.data()[i].ptr();
			components[component->getType().c_str()] = component->getProperties();
		}

		properties["components"] = components;
	}

	{ // Set child properties
		yam2d::PropertySet childs;
		for( size_t i=0; i<m_childs.data().size(); ++i )
		{
			const Entity* child = m_childs.data()[i].ptr();
			childs[i] = child->getProperties();
		}

		properties["childs"] = childs;
	}

    return properties;
}


yam2d::PropertySet Entity::getDefaultProperties()
{
    static yam2d::PropertySet properties = Component::getDefaultProperties();
    properties["type"] = "Entity";
    properties["components"] = yam2d::PropertySet();
    properties["childs"] = yam2d::PropertySet();
    return properties;
}


void Entity::setModified(bool modified)
{
    Component::setModified(modified);
    if( modified == false )
    {
        for(size_t i=0; i<m_components.data().size(); ++i )
        {
            m_components.data()[i]->clearModified();
        }
    }

	if( modified == false )
    {
        for(size_t i=0; i<m_childs.data().size(); ++i )
        {
            m_childs.data()[i]->clearModified();
        }
    }
}


bool Entity::isModified() const
{
    if( Component::isModified() )
        return true;

    for(size_t i=0; i<m_components.data().size(); ++i )
    {
        if( m_components.data()[i]->isModified() )
            return true;
    }

	for(size_t i=0; i<m_childs.data().size(); ++i )
    {
        if( m_childs.data()[i]->isModified() )
            return true;
    }

    return false;
}


void Entity::addComponent(Component* component)
{
    setModified(true);
    m_components.data().push_back(component);
}


const Component* Entity::getComponent(const std::string& type) const
{
    for( size_t i=0; i<m_components.data().size(); ++i )
    {
        if( m_components.data()[i]->getType() == type )
        {
            return m_components.data()[i].ptr();
        }
    }

    return 0;
}


Component* Entity::getComponent(const std::string& type)
{
    for( size_t i=0; i<m_components.data().size(); ++i )
    {
        if( m_components.data()[i]->getType() == type )
        {
            return m_components.data()[i];
        }
    }

    return 0;
}


void Entity::setAllProperties(ComponentFactory* componentFactory, const yam2d::PropertySet& properties)
{
    Component::setAllProperties(properties);

    if( properties.hasProperty("components") )
    {
        // Construct all components from properties
        const yam2d::PropertySet& components = Entity::getPropertiesRef()["components"].get<yam2d::PropertySet>();
       // LOG("Num components: %d", components.size());
        for(int i=0; i<components.size(); ++i )
        {
            const yam2d::PropertySet& newComponentProperties = components[i].get<yam2d::PropertySet>();
			Component* component = componentFactory->createNewComponent(newComponentProperties["type"].get<std::string>(), this, newComponentProperties);
            addComponent(component);
        }
    }

	if( properties.hasProperty("childs") )
    {
        // Construct all childs from properties
        const yam2d::PropertySet& childs = Entity::getPropertiesRef()["childs"].get<yam2d::PropertySet>();
       // LOG("Num childs: %d", childs.size());
        for(int i=0; i<childs.size(); ++i )
        {
			const yam2d::PropertySet& childProperties = childs[i].get<yam2d::PropertySet>();
			Entity* child = componentFactory->createNewEntity(componentFactory, childProperties["type"].get<std::string>(), this, childProperties);
            addChild(child);
        }
    }
}


void Entity::addChild(Entity* child)
{
	assert( child != 0 );
	m_childs.data().push_back(child);
}


void Entity::removeChild(Entity* child)
{
	for( int i=0; i<getChildCount(); ++i )
	{
		Entity* e = getChild(i);
		if( e == child )
		{
			m_childs.data().erase(m_childs.data().begin()+i);
		}
	}
}


int Entity::getChildCount() const
{
	return m_childs.data().size();
}


Entity* Entity::getChild(int index)
{
	assert( index >= 0 && index < getChildCount() );
	return m_childs.data()[index];
}

const Entity* Entity::getChild(int index) const
{
	assert( index >= 0 && index < getChildCount() );
	return m_childs.data()[index].ptr();
}



