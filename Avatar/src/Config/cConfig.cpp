#include "StdAfx.h"
#include <Avatar/Config/cConfig.h>

BEGIN_RTTI( Loom::Avatar::cConfig )
	PROPERTY( SingleAvatar )
	PROPERTY( Orientation )
	PROPERTY( Position )
	PROPERTY( Id1 )
	PROPERTY( Id2 )
	PROPERTY( Controller1 )
	PROPERTY( Controller2 )
	PROPERTY( ViewportVisible1 )
	PROPERTY( ViewportVisible2 )
END_RTTI()

using namespace Loom::Avatar;

/************************************************************************/
cConfig::cConfig()
: SingleAvatar( false ), Orientation( 180.0f )
, Position( 0, 0, 0 ), Id1( 0 ), Id2( 1 )
, Controller1( "Mouse" ), Controller2( "Network" )
/************************************************************************/
{
}
