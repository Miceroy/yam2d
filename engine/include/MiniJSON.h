/*
 * Copyright (c) 2012 Calvin Rien
 *
 * Based on the JSON parser by Patrick van Bergen
 * http://techblog.procurios.nl/k/618/news/view/14605/14863/How-do-I-write-my-own-parser-for-JSON.html
 *
 * Simplified it so that it doesn't throw exceptions
 * and can be used in Unity iPhone with maximum code stripping.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <PropertySet.h>

namespace yam2d {
    /// <summary>
    /// This class encodes and decodes JSON strings.
    /// Spec. details, see http://www.json.org/
    ///
    /// JSON uses Arrays and Objects. These correspond here to the datatypes IList and IDictionary.
    /// All numbers are parsed to doubles.
    /// </summary>
    class Json {
    public:
        /// <summary>
        /// Parses the string json into a value
        /// </summary>
        /// <param name="json">A JSON string.</param>
        /// <returns>An List&lt;object&gt;, a Dictionary&lt;string, object&gt;, a double, an integer,a string, null, true, or false</returns>
        static PropertySet Deserialize(const std::string& json) {
            // save the string for debug information
            if (json.length() == 0) {
                return PropertySet();
            }

            Ref<Property::PropertyValueBase> obj = Parser::Parse(json);
            Ref<Property::ObjectPropertyValue> res = (Property::ObjectPropertyValue*)obj.ptr();
            return res->getValue<PropertySet>();
        }
    private:
        class Parser : Object {
            static const std::string WHITE_SPACE;
            static const std::string WORD_BREAK;

            enum TOKEN {
                TOKEN_NONE,
                TOKEN_CURLY_OPEN,
                TOKEN_CURLY_CLOSE,
                TOKEN_SQUARED_OPEN,
                TOKEN_SQUARED_CLOSE,
                TOKEN_COLON,
                TOKEN_COMMA,
                TOKEN_STRING,
                TOKEN_NUMBER,
                TOKEN_TRUE,
                TOKEN_FALSE,
                TOKEN_NULL_TOKEN
            };

            std::string json;

        public:
            Parser(const std::string& jsonString) {
                json = jsonString;
            }

            static Property::PropertyValueBase* Parse(std::string jsonString)
            {
                Parser p(jsonString);
                return p.ParseValue();
            }

            void Dispose()
            {
                json.clear();
            }

            size_t Read(std::string& text)
            {
                if( text.length() == 0 )
                    return std::string::npos;

                char res = text[0];
                text = text.substr(1,text.length()-1);
                return res;
            }

            size_t Peek(const std::string& text)
            {
                if( text.length() == 0 )
                    return std::string::npos;

                char res = text[0];
                return res;
            }

            Property::ObjectPropertyValue* ParseObject() {
                PropertySet table;

                // ditch opening brace
                Read(json);

                for (;;) {
                    switch (getNextToken()) {
                    case TOKEN_NONE:
                        return 0;
                    case TOKEN_COMMA:
                        continue;
                    case TOKEN_CURLY_CLOSE:
                        return new Property::ObjectPropertyValue(table);
                    default:
                        // name
                        Ref<Property::StringPropertyValue> name = ParseString();
                        if (name == 0 ) {
                            return 0;
                        }

                        // :
                        if (getNextToken() != TOKEN_COLON) {
                            return 0;
                        }
                        // ditch the colon
                        Read(json);

                        Property::PropertyValueBase* val = ParseValue();
                        // value
                        table[name->getValue<std::string>()] = val;//->ptr();//ParseValue();
                        break;
                    }
                }
            }

            Property::ArrayPropertyValue* ParseArray() {

                Property::ArrayPropertyValue* array = new Property::ArrayPropertyValue();

                // ditch opening bracket
                Read(json);

                // [
                bool parsing = true;
                while (parsing) {
                    TOKEN nextToken = getNextToken();

                    switch (nextToken) {
                    case TOKEN_NONE:
                        return 0;
                    case TOKEN_COMMA:
                        continue;
                    case TOKEN_SQUARED_CLOSE:
                        parsing = false;
                        break;
                    default:
                        Property::PropertyValueBase* value = ParseByToken(nextToken);
                        array->addValue(value);
                        break;
                    }
                }

                return array;
            }

           Property::PropertyValueBase* ParseValue() {
               TOKEN nextToken = getNextToken();
               return ParseByToken(nextToken);
            }

          Property::PropertyValueBase* ParseByToken(TOKEN token) {
                switch (token) {
                case TOKEN_STRING:
                    return ParseString();
                case TOKEN_NUMBER:
                    return ParseNumber();
                case TOKEN_CURLY_OPEN:
                    return ParseObject();
                case TOKEN_SQUARED_OPEN:
                    return ParseArray();
                case TOKEN_TRUE:
                    return new Property::BoolPropertyValue(true);
                case TOKEN_FALSE:
                    return new Property::BoolPropertyValue(false);
                case TOKEN_NULL_TOKEN:
                    return 0;
                default:
					break;
                }
                return 0;
            }

            Property::StringPropertyValue* ParseString() {
                std::string s;
                char c;

                // ditch opening quote
                Read(json);

                bool parsing = true;
                while (parsing) {

                    if (Peek(json) == -1) {
                        parsing = false;
                        break;
                    }

                    c = getNextChar();
                    switch (c) {
                    case '"':
                        parsing = false;
                        break;
                    case '\\':
                        if (Peek(json) == -1) {
                            parsing = false;
                            break;
                        }

                        c = getNextChar();
                        switch (c) {
                        case '"':
                        case '\\':
                        case '/':
                            s += c;
                            break;
                        case 'b':
                            s += '\b';
                            break;
                        case 'f':
                            s += '\f';
                            break;
                        case 'n':
                            s += '\n';
                            break;
                        case 'r':
                            s += '\r';
                            break;
                        case 't':
                            s += '\t';
                            break;
                        case 'u':
                            assert(0); // hex not supported
                            break;
                        }
                        break;
                    default:
                        s += c;
                        break;
                    }
                }

                return new Property::StringPropertyValue(s);
            }

            Property::PropertyValueBase* ParseNumber() {
                std::string number = getNextWord();

                if (number.find_first_of('.') == std::string::npos) {
                    int parsedInt = atoi(number.c_str());
                    return new Property::IntPropertyValue(parsedInt);
                }

                float parsedFloat =  (float)atof(number.c_str());
                return new Property::FloatPropertyValue(parsedFloat);
            }

            void EatWhitespace() {
                while (WHITE_SPACE.find_first_of(getPeekChar()) != std::string::npos) {
                    Read(json);

                    if (Peek(json) == std::string::npos) {
                        break;
                    }
                }
            }

            char getPeekChar()
            {
                return (char)Peek(json);
            }

            char getNextChar()
            {
                return (char)Read(json);
            }

            std::string getNextWord()
            {
                std::string word;

                while (WORD_BREAK.find_first_of(getPeekChar()) == std::string::npos)
                {
                    word += getNextChar();

                    if (Peek(json) == std::string::npos)
                    {
                        break;
                    }
                }

                return word;
            }

            TOKEN getNextToken()
            {
                EatWhitespace();

                if (Peek(json) == std::string::npos)
                {
                    return TOKEN_NONE;
                }

                char c = getPeekChar();
                switch (c) {
                case '{':
                    return TOKEN_CURLY_OPEN;
                case '}':
                    Read(json);
                    return TOKEN_CURLY_CLOSE;
                case '[':
                    return TOKEN_SQUARED_OPEN;
                case ']':
                    Read(json);
                    return TOKEN_SQUARED_CLOSE;
                case ',':
                    Read(json);
                    return TOKEN_COMMA;
                case '"':
                    return TOKEN_STRING;
                case ':':
                    return TOKEN_COLON;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '-':
                    return TOKEN_NUMBER;
                }

                std::string word = getNextWord();

                if(word=="false")
                {
                    return TOKEN_FALSE;
                }
                else if (word=="true")
                {
                    return TOKEN_TRUE;
                }
                else if(word=="null")
                {
                    return TOKEN_NULL_TOKEN;
                }

                return TOKEN_NONE;
            }
        };

    public:
        /// <summary>
        /// Converts a IDictionary / IList object or a simple type (string, int, etc.) into a JSON string
        /// </summary>
        /// <param name="json">A Dictionary&lt;string, object&gt; / List&lt;object&gt;</param>
        /// <returns>A JSON encoded string, or null if object 'json' is not serializable</returns>
        static std::string Serialize(const PropertySet& obj) {
            return Serializer::Serialize(&obj);
        }
    private:

        class Serializer {
            std::string json;

        public:
            Serializer() {
            }

            static std::string Serialize(const PropertySet* obj) {
                Serializer instance;
                instance.SerializeObject(*obj);
                return instance.json;
            }

            void SerializeValue(const Property::PropertyValueBase* propVal) {
                const Property::ObjectPropertyValue* objectProp =  dynamic_cast<const Property::ObjectPropertyValue*>(propVal);
                const Property::ArrayPropertyValue* arrayProp = dynamic_cast<const Property::ArrayPropertyValue*>(propVal);
                const Property::StringPropertyValue* stringProp = dynamic_cast<const Property::StringPropertyValue*>(propVal);
                const Property::FloatPropertyValue* floatProp = dynamic_cast<const Property::FloatPropertyValue*>(propVal);
                const Property::IntPropertyValue* intProp = dynamic_cast<const Property::IntPropertyValue*>(propVal);
                const Property::BoolPropertyValue* boolProp = dynamic_cast<const Property::BoolPropertyValue*>(propVal);

                if(propVal == 0 )
                {
                    json += "null";
                }
                else if( objectProp )
                {
                    SerializeObject(objectProp->getValue());
                }
                else if( arrayProp )
                {
                    SerializeArray(arrayProp->getValue());
                }
                else if( floatProp )
                {
                    SerializeFloat(floatProp->getValue());
                }
                else if( intProp )
                {
                    SerializeInt(intProp->getValue());
                }
                else if( stringProp )
                {
                    SerializeString(stringProp->getValue());
                }
                else if( boolProp )
                {
                    json += boolProp->getValue() ? "true" : "false";
                }                
                else
                {
                    assert(0);
                }
            }

            void SerializeObject(const PropertySet& properties) {
                bool first = true;

                json += '{';

                for( PropertySet::const_iterator it=properties.begin(); it != properties.end(); ++it)
                {
                    if (!first)
                    {
                        json += ',';
                    }

                    SerializeString(it->getName());
                    json += ':';

                    SerializeValue( &it->getValue() );

                    first = false;
                }

                json += '}';
            }

            void SerializeArray(const Property::PropertyValueArray& anArray) {
                json += '[';

                bool first = true;

                for( size_t i=0; i<anArray.size(); ++i )
                {
                    const Property::PropertyValueBase* prop = anArray[i].ptr();
                    if (!first)
                    {
                        json += ',';
                    }


                    SerializeValue( prop );

                    first = false;
                }

                json += ']';
            }

            void SerializeString(const std::string& str) {
                json += '\"';

                for( size_t i=0; i<str.length(); ++i )
                {
                    char c = str[i];
                    switch (c)
                    {
                    case '"':
                        json += "\\\"";
                        break;
                    case '\\':
                        json += "\\\\";
                        break;
                    case '\b':
                        json += "\\b";
                        break;
                    case '\f':
                        json += "\\f";
                        break;
                    case '\n':
                        json += "\\n";
                        break;
                    case '\r':
                        json += "\\r";
                        break;
                    case '\t':
                        json += "\\t";
                        break;
                    default:
                        json += c;
                        break;
                    }
                }

                json += '\"';
            }

            void SerializeInt(int val)
            {
                char buf[100];
#if defined(_WIN32)
                sprintf_s(buf,"%d",val);
#else
                sprintf(buf,"%d",val);
#endif
                json += buf;
            }

            void SerializeFloat(float val)
            {
                char buf[100];
#if defined(_WIN32)
                sprintf_s(buf,"%f",val);
#else
                sprintf(buf,"%f",val);
#endif
                json += buf;
            }
        };
    };


}

