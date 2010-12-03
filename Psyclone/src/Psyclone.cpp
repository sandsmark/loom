#include <Psyclone/Psyclone.h>
#include <Psyclone/Module/cModulePsyclone.h>

using namespace Loom::Psyclone;

extern "C"
{

PSYCLONE_API IModule *CreateModule( void )
{
	return new cModulePsyclone();
}

}
