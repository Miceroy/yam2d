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

template < class Type >
inline Ref<Type>::Ref( const Ref& obj )
: m_ptr(obj.m_ptr)
{
	if( m_ptr != 0 )
	{
		obj.m_ptr->addRef();
	}
}

template < class Type >
inline Ref<Type>::Ref( )
: m_ptr(0)
{
}

template < class Type >
inline Ref<Type>::Ref( Type* p )
: m_ptr(p)
{
	if( m_ptr != 0 )
	{
		m_ptr->addRef();
	}
}

template < class Type >
inline Ref<Type>::Ref( int p )
: m_ptr(0)
{
	(void)p;
	assert(p==0); // Not null integer pointer;
}

template < class Type >
inline Ref<Type>::~Ref()
{
	if( m_ptr != 0 )
	{
		if ( 0 == m_ptr->releaseRef() )
		{
			delete m_ptr;
			m_ptr = 0;
		}
	}
}

template < class Type >
inline Ref<Type>& Ref<Type>::operator=( const Ref<Type>& o )
{
	if ( o.m_ptr != 0)
	{
		o.m_ptr->addRef();
	}

	if ( (this->m_ptr != 0) && (0 == this->m_ptr->releaseRef()) )
	{
		delete this->m_ptr;
	}

	this->m_ptr = o.m_ptr;
	return *this;
}


template < class Type >
inline Ref<Type>& Ref<Type>::operator=( Type* o )
{
	if ( o != 0)
	{
		o->addRef();
	}

	if ( (this->m_ptr != 0) && (0 == this->m_ptr->releaseRef()) )
	{
		delete this->m_ptr;
	}

	this->m_ptr = o;
	return *this;
}

template < class Type >
inline Ref<Type>& Ref<Type>::operator=( int o )
{
	(void)o;
	assert(o==0); // Not null integer pointer;

	if ( (this->m_ptr != 0) && (0 == this->m_ptr->releaseRef()) )
	{
		delete this->m_ptr;
	}

	this->m_ptr = 0;
	return *this;
}

template < class Type >
inline Type* Ref<Type>::operator->()
{
	assert( m_ptr != 0 ); // NULL pointer
	return m_ptr;
}

template < class Type >
inline Ref<Type>::operator Type*()
{
	return m_ptr;
}

template < class Type >
inline Type* Ref<Type>::ptr() const
{
	return m_ptr;
}


template < class Type >
inline const Type* Ref<Type>::operator ->() const
{
	assert( m_ptr != 0 ); // NULL pointer
	return m_ptr;
}

template < class Type >
inline bool Ref<Type>::operator ==( const Type* const pObj ) const
{
	return m_ptr == pObj;
}

template < class Type >
inline bool Ref<Type>::operator !=( const Type* const pObj ) const
{
	return m_ptr != pObj;
}

template < class Type >
inline bool Ref<Type>::operator ==( const Ref<Type>& obj ) const
{
	return !(*this != obj);
}

template < class Type >
inline bool Ref<Type>::operator !=( const Ref<Type>& obj ) const
{
	return ( m_ptr != obj.m_ptr );
}

template < class Type >
inline bool Ref<Type>::operator !( ) const
{
	return ( m_ptr == 0 );
}


template < class Type >
inline Type& Ref<Type>::operator* () const
{
	assert( m_ptr != 0 ); // NULL pointer
	return *m_ptr;
}

template < class Type >
inline bool operator ==( Type* lhs, Ref<Type> rhs )
{
	return rhs == lhs;
}

template < class Type >
inline bool operator !=( Type* lhs, Ref<Type> rhs )
{
	return rhs != lhs;
}

