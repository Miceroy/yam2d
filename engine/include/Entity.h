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
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <Object.h>
#include <PropertySet.h>
#if defined(_MSC_VER) //|| !defined(WIN32)

#else
#include <typeinfo>
#endif

namespace yam2d
{

	class Entity;
	class Component;

	// Interface for creating components.
	class ComponentFactory : public Object
	{
	public:
		virtual Component* createNewComponent(const std::string& type, Entity* owner, const yam2d::PropertySet& properties);
		virtual Entity* createNewEntity(ComponentFactory* componentFactory, const std::string& type, Entity* parent, const yam2d::PropertySet& properties);
	};


	template< class T >
	class EntityArray : public yam2d::Object
	{
	public:
		inline EntityArray();

		virtual ~EntityArray();

		std::vector< yam2d::Ref<T> >& data();

		const std::vector< yam2d::Ref<T> >& data() const;

		const std::string& getName() const;

		void setName(const std::string& name);
	private:
		EntityArray(const EntityArray&);

		EntityArray& operator=(const EntityArray&);

		std::string m_name;
		std::vector< yam2d::Ref<T> > m_data;
	};

	template< class T >
	EntityArray<T>::EntityArray()
		: Object()
		, m_name()
		, m_data()
	{
	}

	template< class T >
	EntityArray<T>::~EntityArray()
	{

	}

	template< class T >
	std::vector< yam2d::Ref<T> >& EntityArray<T>::data() { return m_data; }

	template< class T >
	const std::vector< yam2d::Ref<T> >& EntityArray<T>::data() const { return m_data; }

	template< class T >
	const std::string& EntityArray<T>::getName() const { return m_name; }

	template< class T >
	void EntityArray<T>::setName(const std::string& name) { m_name = name; }


	class Entity;

	class Component : public yam2d::Object
	{
	public:
		Component(Entity* owner, const yam2d::PropertySet& properties);

		virtual ~Component();

		Entity* getOwner();

		const Entity* getOwner() const;

		///
		/// \brief getProperties returns properties.
		/// \return const Engine::PropertySet& - Reference to property set
		///
		virtual yam2d::PropertySet getProperties() const;

		static yam2d::PropertySet getDefaultProperties();

		void setType(const std::string& type);

		std::string getType() const;

		virtual bool isModified() const { return m_modified; }
		void clearModified() { setModified(false); }

	protected:
		virtual void setModified(bool modified) { m_modified = modified; }


		/**
		 * @brief Updates given properties to this entity.
		 *
		 * Note: This method does not update child component properties.
		 *
		 * @param properties to update.
		 */
		void setAllProperties(const yam2d::PropertySet& properties);

		yam2d::PropertySet& getPropertiesRef()
		{
			return m_properties;
		}

	private:
		bool m_modified;
		Entity* m_owner;
		yam2d::PropertySet m_properties;
	};



	class Entity : public Component
	{
	public:
		Entity(Entity* parent, ComponentFactory* componentFactory, const yam2d::PropertySet& properties);

		virtual ~Entity();

		Entity* getParent() { return getOwner(); }

		const Entity* getParent() const { return getOwner(); }

		///
		/// \brief getProperties returns properties.
		/// \return const Engine::PropertySet& - Reference to property set
		///
		virtual yam2d::PropertySet getProperties() const;

		static yam2d::PropertySet getDefaultProperties();

		void addComponent(Component* component);

		const Component* getComponent(const std::string& type) const;
		Component* getComponent(const std::string& type);
		std::vector<Component*> getComponents(const std::string& type);

		template<class Type>
		const Type* getComponent() const
		{
#if defined(_MSC_VER) //|| !defined(WIN32)
			const char* const requiredTypeName = typeid(Type).raw_name();
#else
			const char* const requiredTypeName = typeid(Type).name();
#endif
			for (size_t i = 0; i < m_components.data().size(); ++i)
			{
#if defined(_MSC_VER) //|| !defined(WIN32)
				if (strcmp(typeid(*m_components.data()[i].ptr()).raw_name(), requiredTypeName) == 0)
#else
				if( strcmp(typeid(*m_components.data()[i].ptr()).name(),requiredTypeName) == 0 )
#endif
				{
					return dynamic_cast<Type*>(m_components.data()[i].ptr());
				}
			}
			return 0;
		}

		template<class Type>
		Type* getComponent()
		{
#if defined(_MSC_VER) //|| !defined(WIN32)
			const char* const requiredTypeName = typeid(Type).raw_name();
#else
			const char* const requiredTypeName = typeid(Type).name();
#endif
			for (size_t i = 0; i < m_components.data().size(); ++i)
			{
#if defined(_MSC_VER) //|| !defined(WIN32)
				if (strcmp(typeid(*m_components.data()[i].ptr()).raw_name(), requiredTypeName) == 0)
#else
				if( strcmp(typeid(*m_components.data()[i].ptr()).name(),requiredTypeName) == 0 )
#endif
				{
					return dynamic_cast<Type*>(m_components.data()[i].ptr());
				}
			}
			return 0;
		}

		template<class Type>
		std::vector<Type*> getComponents()
		{
			std::vector<Type*> res;
#if defined(_MSC_VER) //|| !defined(WIN32)
			const char* const requiredTypeName = typeid(Type).raw_name();
#else
			const char* const requiredTypeName = typeid(Type).name();
#endif
			for (size_t i = 0; i < m_components.data().size(); ++i)
			{
				//if (*m_components.data()[i].ptr() != 0)
				{
#if defined(_MSC_VER) //|| !defined(WIN32)
					if (strcmp(typeid(*m_components.data()[i].ptr()).raw_name(), requiredTypeName) == 0)
#else
					if (strcmp(typeid(*m_components.data()[i].ptr()).name(), requiredTypeName) == 0)
#endif
					{
						res.push_back(dynamic_cast<Type*>(m_components.data()[i].ptr()));
					}
				}
			}
			return res;
		}

		template<class Type>
		std::vector<Type*> getComponentsOfInterface()
		{
			std::vector<Type*> res;

			for (size_t i = 0; i < m_components.data().size(); ++i)
			{
				if (dynamic_cast<Type*>(m_components.data()[i].ptr()) != 0)
				{
					res.push_back(dynamic_cast<Type*>(m_components.data()[i].ptr()));
				}
			}
			return res;
		}

		virtual bool isModified() const;

		void addChild(Entity* child);

		void removeChild(Entity* child);

		int getChildCount() const;

		Entity* getChild(int index);
		const Entity* getChild(int index) const;

	protected:

		virtual void setModified(bool modified);

		/**
		 * @brief Updates given properties to this entity.
		 *
		 * Note: This method does not update child component properties.
		 *
		 * @param properties to update.
		 */
		void setAllProperties(ComponentFactory* conponentFactory, const yam2d::PropertySet& properties);

	private:
		EntityArray<Component>	m_components;
		EntityArray<Entity>		m_childs;
	};

}

#endif // ENTITY_H_
