#include <Visor/Config/cConfig.h>

BEGIN_RTTI( Loom::Visor::cConfig )
	PROPERTY( ConfigPath )
END_RTTI()

using namespace Loom::Visor;

/************************************************************************/
cConfig::cConfig()
: ConfigPath( _T( "replicode/settings.xml" ) )
/************************************************************************/
{
}
