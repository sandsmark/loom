#pragma once

#include <Core/Datatype/cString.h>

BEGIN_NAMESPACE( Core )

class cClass;

class CORE_API cClassRegistry : public ISingleton<cClassRegistry>
{
protected:
	cHashMap<const cString,const cClass*> mClasses;

public:
	const cClass *FindClass( const cString &iName );
	void Register( const cString &iName, const cClass *iClass );
};

END_NAMESPACE()
