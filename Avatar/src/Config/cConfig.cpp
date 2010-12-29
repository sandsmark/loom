#include <Avatar/Config/cConfig.h>

BEGIN_RTTI( Loom::Avatar::cConfig )
	PROPERTY( SingleAvatar )
	PROPERTY( Orientation )
	PROPERTY( Position )
END_RTTI()

using namespace Loom::Avatar;

/************************************************************************/
cConfig::cConfig()
: SingleAvatar( false ), Orientation( 180.0f )
, Position( 0, 0, 0 )
/************************************************************************/
{
}
