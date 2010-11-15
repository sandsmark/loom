#include <Config/Config.h>
#include <Config/Module/cModuleConfig.h>

using namespace Loom::Config;

extern "C"
{

CONFIG_API IModule *CreateModule( void )
{
	return new cModuleConfig();
}

}
