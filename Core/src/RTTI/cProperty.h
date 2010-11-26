#pragma once

#include <Core/Serializer/ISerializer.h>

BEGIN_NAMESPACE( Core )

class CORE_API cProperty
{
protected:
	cString mName;
	int mOffset;

public:
//	cProperty() : mOffset(0), mName( NULL ) {}
	cProperty( const cString &iName, const int iOffset ) : mOffset( iOffset ), mName( iName ) {}

	const cString &GetName( void ) const { return mName; }

	template<typename T>
	T &GetValue( const IObject *iThis ) const
	{
		return *((T*)( ( (char*)iThis) + mOffset ) );
	}

	template<typename T>
	void SetValue( const IObject *iThis, const T &iValue ) const
	{
		*((T*)( ( (char*)iThis) + mOffset ) ) = iValue;
	}

	virtual void Read( ISerializer *iSerializer, const IObject *iThis ) const = 0;
	virtual void Write( ISerializer *iSerializer, const IObject *iThis ) const = 0;
};

template<typename T>
class cPropertyInstance : public cProperty
{
public:
	cPropertyInstance( const cString &iName, const int iOffset ) : cProperty( iName, iOffset ) {}

	virtual void Read( ISerializer *iSerializer, const IObject *iThis ) const
	{
		iSerializer->Read<T>( GetValue<T>( iThis ) );
	}
	virtual void Write( ISerializer *iSerializer, const IObject *iThis ) const
	{
		iSerializer->Write( &GetValue<T>( iThis ) );
	}
};

END_NAMESPACE()
