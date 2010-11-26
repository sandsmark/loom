#pragma once

#include <Core/RTTI/IObject.h>

BEGIN_NAMESPACE( Core )

template< typename T >
class cObject : public IObject
{
public:
	cObject() : IObject( &cClass::GetRTTI<T>() ) {}
};


END_NAMESPACE()
