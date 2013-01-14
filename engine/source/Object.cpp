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

#include <Object.h>

#include <stdio.h>
#include <typeinfo>

#include <es_util.h>

//#define MEMORY_LEAK_DEBUGGING
#define SHOW_LEAKS
//#define ASSERT_ON_LEAKS

namespace yam2d
{
namespace
{
    class RefCounter
    {
    public:
        RefCounter()
            : refs(0)
        {
        }


        ~RefCounter()
        {
#if defined(SHOW_LEAKS)
            if( refs != 0 )
            {
				esLogMessage("Memory leak detected!\n");
#if defined(MEMORY_LEAK_DEBUGGING)
                for( size_t i=0; i<m_objects.size(); ++i )
                {
                    esLogMessage(m_objectNames[i].c_str());
                }
#endif
            }
            else
            {
                esLogMessage("No memory leaks detected!\n");
            }
#if defined(ASSERT_ON_LEAKS)
            assert( refs == 0 );
#endif
#endif
        }

        void add(Object* o, const char* const name)
        {
#if defined(MEMORY_LEAK_DEBUGGING)
            m_objects.push_back(o);
            m_objectNames.push_back(name);
#else
			(void)name;
			(void)o;
#endif
            ++refs;
        }

        void release(Object* o)
        {
#if defined(MEMORY_LEAK_DEBUGGING)
            size_t index = 0;
			
			for( ; index<m_objects.size(); ++index )
			{
				if( m_objects[index] == o )
				{
					break;
				}
			}

			m_objects.erase( m_objects.begin() + index );
			m_objectNames.erase( m_objectNames.begin() + index );
#else
			(void)o;
#endif
            --refs;
        }

    private:
#if defined(MEMORY_LEAK_DEBUGGING)
        std::vector<Object*> m_objects;
        std::vector<std::string> m_objectNames;
#endif
        int refs;
    };

    static RefCounter refs;
}

Object::Object(/*const char* const name*/)
: m_numOfRefs(0)
{
    refs.add(this,"");
}

Object::~Object()
{
    refs.release(this);
    if( this->m_numOfRefs != 0 )
    {
        assert( this->m_numOfRefs == 0 ); // "Can not delete Object, when it have references some where else";
    }
}



}



