#include <Visor/Visor.h>
#include <Visor/Module/cModuleVisor.h>

using namespace Loom::Visor;

extern "C"
{

VISOR_API IModule *CreateModule( void )
{
	return new cModuleVisor();
}

}
