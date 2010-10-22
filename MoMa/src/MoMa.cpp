#include <MoMa/MoMa.h>
#include <MoMa/Module/cModuleMoMa.h>

using namespace Loom::MoMa;

MOMA_API IModule *CreateModule( void )
{
	return new cModuleMoMa();
}
