// Copyright (C) 2012 Mikko Romppainen. All rights reserved.
#ifndef REF_H_
#define REF_H_

namespace yam2d
{

/**
 * Smart pointer class.
 * The main purpose of this class is to destroy automatically some dynamically allocated resources.
 * Ref class is a template class. Template class takes one parameter Type:
 *
 * - Type is the type for a pointer which is stored in the Ref. Ref requires few operations from used type:
 *		- void Type::addRef(),					Increases Reference count of an object.
 *		- void Type::releaseRef(),				Decreases Reference count of an object. Returns object new Reference count.
 *		- void operatot delete( size_t ),		Standard format delete operator.
 *		.
 * .
 *
 * Note that yam2d::Object implements all required operators, so objects that are inherited from
 * yam2d::Object are automatically compatible with Ref.
 *
 * @see yam2d::Object
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com)
 */
template < class Type >
class Ref
{
public:
	/**
	 * Copy constructor. Increases object Ref count.
	 */
	inline Ref( const Ref& obj );

	/**
	 * Default constructor. Sets member pointer to 0.
	 */
	inline Ref( );

	/**
	 * Constructor for pointer. Increases object Ref count.
	 */
	inline Ref( Type* p );
	inline Ref( int p );

	/**
	 * Destructor. Decreases object Ref count and if this Ref is last, object is deleted.
	 */
	inline ~Ref<Type>();

	/**
	 * Assigment operator for other Ref of same type.
	 */
	inline Ref<Type>& operator=( const Ref<Type>& o );
	inline Ref<Type>& operator=( Type* o );
	inline Ref<Type>& operator=( int o );
	
	/**
	 * Operator for pointer member access "->".
	 */
	inline Type* operator->();

	/**
	 * Conversion operator for accessing pointer from Ref.
	 */
#if !defined(SWIG)
	inline operator Type*();
#endif
	
	/**
	 * Method for accessing pointer from Ref.
	 */
	inline Type* ptr() const;

	/**
	 * Operator for pointer member access "->".
	 */
	inline const Type* operator ->() const;

	/**
	 * Equals to operator compares Ref to a pointer. Returns true if pointers are same.
	 */
	inline bool operator ==( const Type* const pObj ) const;

	/**
	 * Not equals to operator compares Ref to a pointer. Returns true if pointers are not same.
	 */
	inline bool operator !=( const Type* const pObj ) const;

	/**
	 * Equals to operator compares Ref to another Ref. Returns true if Refs point to the same pointer.
	 */
	inline bool operator ==( const Ref<Type>& obj ) const;

	/**
	 * Not equals to operator compares Ref to another Ref. Returns true if Refs does not point to the same pointer.
	 */
	inline bool operator !=( const Ref<Type>& obj ) const;

	/**
	 * Not operator compares member pointer to zero. Returns true if pointer is 0.
	 */
	inline bool operator !( ) const;

	/**
	 * "Star" operator for pointer object access. Whit this method content of the pointer can be accessed with regular *-notation.
	 */
	inline Type& operator* () const;

private:
	// Member variables
	Type*		m_ptr;

};

/*
 * Equals to operator compares pointer to a Ref. Returns true if pointers are same.
 */
template < class Type >
inline bool operator ==( Type* lhs, Ref<Type> rhs );

/*
 * Not equals to operator compares pointer to a Ref. Returns true if pointers are not same.
 */
template < class Type >
inline bool operator !=( Type* lhs, Ref<Type> rhs );

#include <Ref.inl>

}



#endif // REF_H_


