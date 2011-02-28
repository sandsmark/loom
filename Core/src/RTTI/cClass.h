#pragma once

#include <Core/RTTI/cProperty.h>
#include <Core/Container/cArray.h>

BEGIN_NAMESPACE( Core )

//template<typename T>
class CORE_API cClass
{
protected:
	cString mName;
	cArray<cProperty*> mProperties;

public:
	cClass( const cString &iName ) : mName( iName ) {}

	const size_t GetNumProperties( void ) const { return mProperties.GetSize(); }
	const cProperty &GetProperty( const size_t iIndex ) const { return *mProperties[ iIndex ]; }
	const cProperty *FindProperty( const cString &iName ) const
	{
		for ( size_t i=0; i<mProperties.GetSize(); i++ )
		{
			const cProperty *vProperty = mProperties[i];
			if ( vProperty->GetName() == iName ) return vProperty;
		}

		return NULL;
	}

	const cString &GetName( void ) const { return mName; }

	template<class T>
	static const cClass &GetRTTI( void );

	virtual IObject *Create( void ) const = 0;

	/*
	template<class T>
	static const cClass &GetRTTI( const T *iDummy )
	{
		return GetRTTI<T>();
	}
	*/
};

template<class iClass>
class cClassInstance : public cClass
{
private:
	class cInitializer
	{
	public:
		cInitializer();
	};
	static cInitializer mInitializer;

protected:
	typedef iClass tThisClass;

	template<typename T>
	void AddProperty( const cString &iName, int iOffset, const T & )
	{
		mProperties.Add( new cPropertyInstance<T>( iName, iOffset ) );
//		mProperties.Add( new cProperty( iName, iOffset ) );
	}

public:
	cClassInstance( const cString &iName );

	virtual IObject *Create( void ) const
	{
		return new iClass;
	}
};

#define BEGIN_RTTI( iClass )	\
	template<> const Loom::Core::cClass &Loom::Core::cClass::GetRTTI<iClass>()	\
	{	\
		static const Loom::Core::cClassInstance<iClass> vRTTI( Loom::Core::cClassInstance<iClass>( L#iClass ) );	\
		\
		return vRTTI;	\
	}	\
	Loom::Core::cClassInstance<iClass>::cInitializer Loom::Core::cClassInstance<iClass>::mInitializer;	\
	template<> Loom::Core::cClassInstance<iClass>::cInitializer::cInitializer()	\
	{	\
		Loom::Core::cClassRegistry::Get().Register( L#iClass, &Loom::Core::cClass::GetRTTI<iClass>() );	\
	}	\
	template<>	\
	Loom::Core::cClassInstance<iClass>::cClassInstance( const cString &iName )	\
	: Loom::Core::cClass( iName ) \
	{

#define END_RTTI()	\
	}

// Property macro must be used inside BEGIN_RTTI and END_RTTI
#define PROPERTY( iName )	\
	AddProperty( L#iName, ((int)&(((tThisClass*)4242)->iName)-((int)((IObject*)(tThisClass*)4242))), ((tThisClass*)NULL)->iName );
		

END_NAMESPACE()
