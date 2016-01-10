// Copyright (C) 2015 Mikko Romppainen. See license.txt for license terms.

#include <PropertySet.h>
#include <algorithm>
#include <cstdarg>
#include <MiniJSON.h>


namespace yam2d
{

#if !defined (assert_message)
#define assert_message(e,msg) (msg); assert(e)
#endif



const std::string Json::Parser::WHITE_SPACE = " \t\n\r";
const std::string Json::Parser::WORD_BREAK = " \t\n\r{}[],:\"";

namespace
{
	std::string app_sprintf(const char* const formatStr, ... )
    {
        va_list params;
        char buf[BUFSIZ];

        va_start ( params, formatStr );
#if defined(_WIN32)
        vsprintf_s ( buf, sizeof(buf),  formatStr, params );
#else
        vsprintf( buf,  formatStr, params );
#endif

        std::string res;
        res += buf;
        va_end ( params );
        return res;
    }

	
	std::string intToString(int val)
	{
		return app_sprintf("%d",val);
	}
}



Property::Property(const std::string& name)
	: Object()
	, m_name(name)
	, m_property(0)
    //, m_attributes(0)
{
}

Property::~Property()
{
	m_property = 0;
    //m_attributes = 0;
}

Property::Property(const Property& o)
	: Object()
	, m_name(o.m_name)
	, m_property(o.m_property)
    //, m_attributes(0)
{
    //if( o.m_attributes.ptr() )
    //{
    //	setAttributes(*o.m_attributes);
    //}
}

Property& Property::operator=(const Property& o)
{
	if( this != &o )
	{
		this->m_name = o.m_name;
		this->m_property = o.m_property;
//		setAttributes(*o.m_attributes);
	}

	return *this;
}
		
Property& Property::operator=(const char* value)
{
    std::string val;
	val = value;
	(*this) = val;
	return *this;
}

Property& Property::operator=(char* value)
{
    std::string val;
	val = value;
	(*this) = val;
	return *this;
}

bool Property::isVoid() const
{
	return this->m_property == 0;
}
	
bool Property::isChildPropertySet() const
{
	return isTypeOf<PropertySet>();
}

const Property& Property::operator[](int index) const
{
    assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
    PropertyValue<PropertySet>* res = dynamic_cast< PropertyValue<PropertySet>* >(this->m_property.ptr());
    assert_message(res != 0, "Property \"" + getName() + "\" is not type of PropertySet");
	PropertySet& props = res->getValue<PropertySet&>();
	return props[index];
}

const Property& Property::operator[](const char* const index) const
{
    assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
    PropertyValue<PropertySet>* res = dynamic_cast< PropertyValue<PropertySet>* >(this->m_property.ptr());
    assert_message(res != 0, "Property \"" + getName() + "\" is not type of PropertySet");
	PropertySet& props = res->getValue<PropertySet&>();
	return props[index];
}

Property& Property::operator[](int index)
{
	if( this->m_property == 0)
	{
        this->m_property = new PropertyValue< PropertySet >( PropertySet() );
	}

    assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
    PropertyValue<PropertySet>* res = dynamic_cast< PropertyValue<PropertySet>* >(this->m_property.ptr());
    assert_message(res != 0, "Property \"" + getName() + "\" is not type of PropertySet");
	PropertySet& props = res->getValue<PropertySet&>();
	return props[index];
}

Property& Property::operator[](const char* const index)
{
	if( this->m_property == 0)
	{
        this->m_property = new PropertyValue<PropertySet>( PropertySet() );
	}

    assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
    PropertyValue<PropertySet>* res = dynamic_cast< PropertyValue<PropertySet>* >(this->m_property.ptr());
    assert_message(res != 0, "Property \"" + getName() + "\" is not type of PropertySet");
    PropertySet& props = res->getValue<PropertySet&>();
    Property& prop = props[index];
    assert_message(prop.getName()==index, "String index mismatch in property \"" + getName() + "\"");
    return prop;
}
		
bool Property::operator==(const std::string& rhs) const
{
	return m_name == rhs;
}


const std::string Property::getAsString() const
{
    std::string res;
	
	if( isTypeOf<float>() )
	{
        res = app_sprintf("%f", get<float>() );
	}
	else if( isTypeOf<int>() )
	{
        res = app_sprintf("%d", get<int>() );
	}
    else if( isTypeOf<std::string>() )
    {
        assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
        PropertyValue<std::string>* r = dynamic_cast< PropertyValue<std::string>* >(this->m_property.ptr());
        assert_message(r != 0, "Property \"" + getName() + "\" is not type of string");
        return r->getValue<std::string>();
	}
	
	return res;
}

const bool Property::getAsBool() const
{	
	if( isTypeOf<int>() )
	{
		if( get<int>()==0 )
			return false;
		if( get<int>()==1 )
			return true;
	}
	else if( isTypeOf<std::string>() )
	{
		if( get<std::string>()=="false" )
			return false;
		if( get<std::string>()== "true" )
			return true;
	}
    
	assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
    assert_message(0, "Property \"" + getName() + "\" is not type of bool");
    return false;
}

const float Property::getAsFloat() const
{
    assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );

	// Try as float
	{
        PropertyValue<float>* res = dynamic_cast< PropertyValue<float>* >(this->m_property.ptr());
		if( res != 0 )
		{
			return res->getValue<float>();
		}
	}

	// Try as double
	{
        PropertyValue<double>* res = dynamic_cast< PropertyValue<double>* >(this->m_property.ptr());
		if( res != 0 )
		{
			return (float)res->getValue<double>();
		}
	}

	// Try as int
	{
        PropertyValue<int>* res = dynamic_cast< PropertyValue<int>* >(this->m_property.ptr());
		if( res != 0 )
		{
			return (float)res->getValue<int>();
		}
	}
    assert_message(0, "Unknown property type in property \"" + getName() + "\"" );
 	return 0;
}

bool Property::isFloat() const
{
    assert_message(this->m_property != 0, "Null property in \"" + getName() + "\"" );
    return 0 != dynamic_cast< PropertyValue<float>* >(this->m_property.ptr())
        || 0 != dynamic_cast< PropertyValue<double>* >(this->m_property.ptr())
        || 0 != dynamic_cast< PropertyValue<int>* >(this->m_property.ptr());
}


PropertySet::PropertySet()
	: Object()
	, m_properties()
{
}

PropertySet::PropertySet(const PropertySet& o)
	: Object()
    , m_properties(o.m_properties)
{
   // std::string json = Json::Serialize(o);
   // PropertySet ps = Json::Deserialize(json);
   // m_properties = ps.m_properties;
}

PropertySet& PropertySet::operator=(const PropertySet& o)
{
	if( this != &o )
	{
		this->m_properties = o.m_properties;
	}

	return *this;
}

PropertySet::~PropertySet()
{
	m_properties.clear();
}


PropertySet PropertySet::createFromJson(const std::string& json)
{
	return Json::Deserialize(json);
}


Property* PropertySet::insertNewProperty(const char* const name)
{
	Property prop = Property(name);
	m_properties.push_back( prop );
	return &m_properties[m_properties.size()-1];
}

Property& PropertySet::operator [](const char* const name)
{
    return operator[]( std::string(name) );
}

const Property& PropertySet::operator [](const char* const name) const
{
    return operator[]( std::string(name) );
}

Property& PropertySet::operator [](const std::string& name)
{	
	PropertySetType::iterator it = findByName(name);

	if( it == m_properties.end())
	{
		Property prop = Property(name);
		m_properties.push_back( prop );
	}
		
	it = findByName(name);
    assert_message( it != m_properties.end(), "Property \"" + name + "\" not found!" );
	return *it;
}

const Property& PropertySet::operator [](const std::string& name) const
{	
	PropertySetType::const_iterator it = findByName(name);
    if( it == m_properties.end() )
    {
        LOG_ERROR("Property \"%s\" not found!", name.c_str());
        LOG_ERROR("  Property canditates:" );
        for(PropertySetType::const_iterator it2=m_properties.begin(); it2!=m_properties.end(); ++it2)
        {
            LOG_ERROR("    %s", (*it2).getName().c_str() );
        }
    }
    assert_message( it != m_properties.end(), "Property \"" + name + "\" not found!" );
	return *it;
}

Property& PropertySet::operator [](int index)
{
	PropertySetType::iterator it = findByIndex(index);

	if( it == m_properties.end())
	{
        std::string name;
        name = app_sprintf("%d", index);
		m_properties.push_back( Property(name) );
	}

		
    it = findByIndex(index);
    assert_message( it != m_properties.end(), "Property \"" + intToString(index) + "\" not found!" );
	return *it;
}

const Property& PropertySet::operator [](int index) const
{
    PropertySetType::const_iterator it = findByIndex(index);
    assert_message( it != m_properties.end(), "Property \"" + intToString(index) + "\" not found!" );
	return *it;
}

PropertySet::PropertySetType::const_iterator PropertySet::findByName(const std::string& name) const
{
    PropertySetType::const_iterator it = std::find( m_properties.begin(), m_properties.end(), name);
	return it;
}

PropertySet::PropertySetType::iterator PropertySet::findByName(const std::string& name)
{
    PropertySetType::iterator it = std::find( m_properties.begin(), m_properties.end(), name);
	return it;
}

PropertySet::PropertySetType::const_iterator PropertySet::findByIndex(int n) const
{
    for( PropertySet::PropertySetType::const_iterator it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        if( n == 0 )
            return it;

        --n;
    }

    return m_properties.end();
}

PropertySet::PropertySetType::iterator PropertySet::findByIndex(int n)
{
    for( PropertySet::PropertySetType::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        if( n == 0 )
            return it;

        --n;
    }

    return m_properties.end();
}

PropertySet::const_iterator PropertySet::begin() const
{
	return m_properties.begin();
}

PropertySet::const_iterator PropertySet::end() const
{
	return m_properties.end();
}
	
PropertySet::iterator PropertySet::begin()
{
	return m_properties.begin();
}

PropertySet::iterator PropertySet::end()
{
	return m_properties.end();
}

void PropertySet::setValues( const std::map< std::string, std::string >& v )
{
	typedef std::map< std::string, std::string > MapType;
	m_properties.clear();
	/*
	m_properties = v;*/
	for( MapType::const_iterator it=v.begin(); it!=v.end(); ++it )
	{
		(*this)[it->first] = it->second;
	}
}


std::string PropertySet::getLiteralProperty(const std::string &name) const 
{
	return (*this)[name].get<std::string>();
}


int PropertySet::getNumericProperty(const std::string &name) const 
{
	return (*this)[name].get<int>();
}

bool PropertySet::hasProperty(const std::string& name) const
{
    PropertySet::const_iterator it = findByName(name);
    if( it == end() )
        return false;

    return true;
}


#if 0 // Qt
PropertySet PropertySet::readFromFile(const std::string& filename)
{

    QFile f(filename.c_str());
    if( false == f.open(QIODevice::ReadOnly) )
    {
        ERROR( "Couldn't open file: \"%s\"", filename.c_str() );
        assert_message(0);
    }

    QByteArray array = f.readAll();

    f.close();

    std::string jsonString = "";

    for( int i=0; i<array.size(); ++i )
    {
        jsonString += (char)array[i];
    }

    if ( jsonString.length() < 4 )
    {
        ERROR( "File: \"%s\" does not have enought data for json", filename.c_str() );
        assert_message(0);
    }

    Json json;
    return json.Deserialize(jsonString);
}

void PropertySet::writeToFile(const PropertySet& properties,const std::string& filename)
{
    Json json;
    std::string jsonString = json.Serialize(properties);

    QFile f(filename.c_str());
    f.open(QIODevice::WriteOnly);
    f.write(jsonString.c_str(),jsonString.length());
    f.close();
}
#else
PropertySet PropertySet::readFromFile(const std::string& filename)
{
	FILE* file = 0;
	fopen_s(&file,filename.c_str(),"r");
	
	if( file == 0 )
	{
		esLogMessage( "File: \"%s\" could not be opened\n", filename.c_str() );
		return PropertySet();		
	}

	// Read all data from file
	fseek (file, 0, SEEK_END);
	long length = ftell (file);
	fseek(file, 0, SEEK_SET);
	std::vector<char> data;
	data.resize(length);
	fread (&data[0], 1, data.size(), file);
	fclose(file);

	// Convert to std::string
    std::string jsonString = &data[0];

    if ( jsonString.length() < 4 )
    {
		esLogMessage( "File: \"%s\" does not have enought data for json\n", filename.c_str() );
        return PropertySet();
    }

    return Json::Deserialize(jsonString);
}

void PropertySet::writeToFile(const PropertySet& properties,const std::string& filename)
{
    std::string jsonString = Json::Serialize(properties);

	FILE* file = 0;
	fopen_s(&file,filename.c_str(),"w");
	
	if( file == 0 )
	{
		esLogMessage( "File: \"%s\" could not be opened\n", filename.c_str() );
		return;		
	}

	fwrite( jsonString.c_str(), 1, jsonString.length(), file);
	fclose(file);
}
#endif

PropertySet PropertySet::clone() const
{
    std::string str = Json::Serialize(*this);
    return Json::Deserialize(str);
}

void PropertySet::updateProperties(const yam2d::PropertySet& from)
{
	yam2d::PropertySet copy = from.clone();

	for( int i=0; i<copy.size(); ++i )
	{
		if( (*this)[copy[i].getName()].isTypeOf<PropertySet>() )
		{
			assert( copy[i].isTypeOf<PropertySet>() ); // Both not property sets.
			// Update PropertySet
			(*this)[copy[i].getName()].get<PropertySet>().updateProperties(copy[i].get<PropertySet>());
		}
		else
		{
			// Not property set but property, update it.
			(*this)[copy[i].getName()] = copy[i].getValuePtr();
		}
	}
}


std::string PropertySet::toString() const
{
	return Json::Serialize(*this);
}

}


