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
#ifndef PROPERTY_SET_H_
#define PROPERTY_SET_H_

#include <string>
#include <map>
#include <Object.h>

namespace yam2d
{


/**
 * Class for properties.
 */ 
class PropertySet : public Object
{
public:
	PropertySet();
	PropertySet( const PropertySet& );
	PropertySet& operator=( const PropertySet& );

	virtual ~PropertySet() {}


	// Set a literal property (string).
	void setLiteralProperty(const std::string &name, const std::string& value);
	
	void setValues( const std::map< std::string, std::string >& v );

	// Get a numeric property (integer).
	int getNumericProperty(const std::string &name) const;

	// Get a literal property (string).
	std::string getLiteralProperty(const std::string &name) const;

	// Returns the amount of properties.
	int getSize() const { return properties.size(); }
	bool hasProperty(const std::string &name) const;
	// Returns the STL map of the properties.
	std::map< std::string, std::string > getList() const 
	{ return properties; }

	// Returns whether there are no properties.
	bool isEmpty() const { return properties.empty(); }

private:
	std::map< std::string, std::string > properties;

};


}


#endif


