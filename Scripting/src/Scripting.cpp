#include <Scripting/Scripting.h>
#include <Scripting/Module/cModuleScripting.h>

using namespace Loom::Scripting;

extern "C"
{

SCRIPTING_API IModule *CreateModule( void )
{
	return new cModuleScripting();
}

}
