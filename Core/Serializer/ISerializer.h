#pragma once

#include <Core/Core.h>
#include <Core/RTTI/IObject.h>

BEGIN_NAMESPACE( Core )

class CORE_API ISerializer
{
public:
	virtual void Serialize( const IObject *iObject ) = 0;
	virtual IObject *Deserialize( void ) = 0;

	template<typename T>
	void Read( T &iTarget );

	template<typename T>
	void Write( const T *iValue );
};

END_NAMESPACE()
