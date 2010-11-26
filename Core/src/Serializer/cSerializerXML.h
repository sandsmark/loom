#pragma once

#include <Core/Serializer/ISerializer.h>

BEGIN_NAMESPACE( Core )

class CORE_API cSerializerXML : public ISerializer
{
	friend class ISerializer;

protected:
	std::wiostream &mStream;

	template< typename T >
	void Write( const T &iElement );

public:
	cSerializerXML( std::wiostream &iStream ) : mStream( iStream ) {}

	cSerializerXML &operator =( const cSerializerXML & )
	{
		throw "NO WAY!";
	}

	virtual void Serialize( const IObject *iObject );
	virtual IObject *Deserialize( void );
};

END_NAMESPACE()
