// Copyright (C) 2015 Mikko Romppainen. See license.txt for license terms.
#ifndef CORE_PROPERTY_SET_H_
#define CORE_PROPERTY_SET_H_

#include <Object.h>
#include <Ref.h>
#include <es_util.h>

#include <string>
#include <map>

namespace yam2d
{

class PropertySet;

#if !defined (LOG_ERROR)
#define LOG_ERROR esLogMessage
#endif

/**
 * Property is a class for accessing value of named property. Property 
 * has a name and value. Value can be accessed using get-methods or in 
 * case of array type of property, it can be accessed by []-operator.
 */
class Property : public Object
{
public:
    class PropertyValueBase : public Object
    {
    protected:
        PropertyValueBase()
        {
        }
    public:
        virtual ~PropertyValueBase()
        {
        }
    };

	template<class T> 
    class PropertyValue : public PropertyValueBase
	{
	public:
        PropertyValue(const T& v)
            : PropertyValueBase()
			, value(v)
        {
		}


        PropertyValue()
            : PropertyValueBase()
            , value()
        {
        }

        virtual ~PropertyValue()
        {
		}
		
		template<class Type>
		void setValue(Type v)
		{
			value = v;
		}

        template<class Type>
		const Type getValue() const
		{
			return value;
		}

		template<class Type>
		Type getValue()
		{
			return value;
		}

        const T getValue() const
        {
            return value;
        }

        T getValue()
        {
            return value;
        }

		const T& getValueConstRef() const
		{
			return value;
		}

		T& getValueRef()
		{
			return value;
		}

		template<class Type>
		void addValue(Type v)
        {
            value.push_back(v);
        }
    private:
        PropertyValue(const PropertyValue&);
        PropertyValue& operator=(const PropertyValue&);
	private:
		T value;
	};

public:
    typedef std::vector< Ref<PropertyValueBase> > PropertyValueArray;

    typedef PropertyValue<bool> BoolPropertyValue;
    typedef  PropertyValue<std::string> StringPropertyValue;
    typedef  PropertyValue<float> FloatPropertyValue;
    typedef  PropertyValue<int> IntPropertyValue;
    typedef  PropertyValue< PropertySet > ObjectPropertyValue;
    typedef  PropertyValue< PropertyValueArray > ArrayPropertyValue;


	/** Initializes property by name. */
    Property(const std::string& name);

	/** Copy constructor. Initializes property by key and value from other property. */
	Property(const Property& o);

	virtual ~Property();

	Property& operator=(const Property& o);
	Property& operator=(const char* value);
	Property& operator=(char* value);
	Property& operator=(int value) { return assign<int>(value); }
	Property& operator=(float value) { return assign<float>(value); }
    Property& operator=(const std::string& value) { return assign<std::string>(value); }
    Property& operator=(Property::PropertyValueBase* value) { m_property = value; return *this; }
	Property& operator=(const PropertySet& value)	{ return assign<PropertySet>(value); }
    Property& operator=(const PropertyValueArray& values)	{ return assign<PropertyValueArray>(values); }

	/** Returns true, if property is empty (property value is not set) */
	bool isVoid() const;
	
	/** Returns true, if property is property set, which contains child properties. */
    bool isChildPropertySet() const;
	
	/** Returns true, if property is type of the template argument. */
    template<class T>
	inline bool isTypeOf() const 
	{
	//	assert(this->m_property != 0);
        PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >( this->m_property.ptr() );
		return res != 0;
    }


	/** Returns value as type of the template argument. If property type is not correct, method asserts in debug mode. */
    template<class T>
	inline const T& get() const 
	{
        if(this->m_property == 0)
        {
            LOG_ERROR("Property %s does not exist", this->getName().c_str() );
            assert(0);
        }
		PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        if(res == 0)
        {
            LOG_ERROR("Property %s is not correct type", this->getName().c_str() );
            assert(0);
        }

		return res->getValueConstRef();
	}

    template<class T>
    inline T& get()
    {
        if(this->m_property == 0)
        {
            LOG_ERROR("Property %s does not exist", this->getName().c_str() );
            assert(0);
        }
        PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        if(res == 0)
        {
            LOG_ERROR("Property %s is not correct type", this->getName().c_str() );
            assert(0);
        }

        return res->getValueRef();
    }

    inline const PropertyValueBase& getValue() const
    {
        assert(this->m_property != 0);
       // PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        //assert( res != 0 );
        //return res->getValue<T&>();
        return *this->m_property.ptr();
    }
	    
	inline PropertyValueBase* getValuePtr()
    {
        assert(this->m_property != 0);
       // PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        //assert( res != 0 );
        //return res->getValue<T&>();
        return this->m_property.ptr();
    }

	const Property& operator[](int index) const;

	const Property& operator[](const char* const index) const;

	Property& operator[](int index);

	Property& operator[](const char* const index);

    bool operator==(const std::string& rhs) const;

	/** Returns the name of the property. */
    const std::string& getName() const
	{
		return m_name;
	}

    /**
     * @brief getUserData
     * Sets user data (Application data) to property. User data can be
     * for example ui widged information etc. User data is NOT saves to json during serialization.
     */
    //void setUserData(Object* userData) { m_userData = userData; }

    /**
     * @brief getUserData
     * @return User data (Application data).
     */
    //const Object* getUserData() const { return m_userData.ptr(); }

/*	bool hasAttributes() const;
	const PropertySet& attributes() const;
	PropertySet& attributes();
    void setAttributes(const PropertySet& attributes);*/
private:
	template<class T>
	Property& assign(const T& value)
	{
        this->m_property = new PropertyValue<T>(value);
		return *this;
	}
	
    const std::string getAsString() const;
    const bool getAsBool() const;
	const float getAsFloat() const;
	bool isFloat() const;

    std::string		m_name;
   // Ref<Object>			m_userData;
    Ref<PropertyValueBase>			m_property;
//	Ref<PropertySet>	m_attributes;
	
};

template<>
inline bool Property::isTypeOf<float>() const 
{
	return isFloat();
}

template<>
inline const float& Property::get() const 
{
	static float v = 0.0f;
	v = getAsFloat();
	return v;
}

template<>
inline const std::string& Property::get() const
{
    static std::string v = "";
	v = getAsString();
	return v;
}

template<>
inline const bool& Property::get() const 
{
	static bool v = false;
	v = getAsBool();
	return v;
}

template<>
inline bool& Property::get() 
{
	static bool v = false;
	v = getAsBool();
	return v;
}

/**
 * Property set is a collection of properties. You can access and set different 
 * properties by name using []-operator. If you want to iterate whole property set, 
 * then you can use begin and end -methods for that. 
 */
class PropertySet : public Object
{
public:
    typedef std::vector< Property > PropertySetType;
	typedef PropertySetType::const_iterator const_iterator;
	typedef PropertySetType::iterator iterator;
	
	PropertySet();

	PropertySet(const PropertySet& o);

	PropertySet& operator=(const PropertySet& o);

	virtual ~PropertySet();
	
	static PropertySet createFromJson(const std::string& json);

	Property* insertNewProperty(const char* const name);

	Property& operator [](const char* const name);

	const Property& operator [](const char* const name) const;

    Property& operator [](const std::string& name);

    const Property& operator [](const std::string& name) const;

	Property& operator [](int index);

	const Property& operator [](int index) const;
	
	const_iterator begin() const;
	const_iterator end() const;
	
	iterator begin();
	iterator end();

	void setValues( const std::map< std::string, std::string >& v );

	// Get a numeric property (integer).
	int getNumericProperty(const std::string &name) const;

	// Get a literal property (string).
	std::string getLiteralProperty(const std::string &name) const;

	int size() const { return m_properties.size(); }

    bool hasProperty(const std::string& name) const;

	template<class T> 
	const T& getOrDefault(const std::string& name, const T& defaultValue) const
	{
		PropertySet::const_iterator it = findByName(name);
		if (it == end())
			return defaultValue;

		return it->get<T>();
	}

    static PropertySet readFromFile(const std::string& filename);

    static void writeToFile(const PropertySet& properties,const std::string& filename);

    PropertySet clone() const;

	void updateProperties(const yam2d::PropertySet& from);

	std::string toString() const;


private:
	PropertySetType		m_properties;

    const_iterator findByName(const std::string& name) const;
    iterator findByName(const std::string& name);

	const_iterator findByIndex(int n) const;
	iterator findByIndex(int n);
};

}

#endif

