#include <Core/RTTI/cClassRegistry.h>

using namespace Loom::Core;

/************************************************************************/
const cClass *cClassRegistry::FindClass( const cString &iName )
/************************************************************************/
{
	const cClass **vResult = mClasses.Find( iName );

	return ( vResult == NULL ) ? NULL : *vResult;
}

/************************************************************************/
void cClassRegistry::Register( const cString &iName, const cClass *iClass )
/************************************************************************/
{
	mClasses[ iName ] = iClass;
}
