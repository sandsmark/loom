#include <Network/Network.h>
#include <Network/Module/cModuleNetwork.h>

using namespace Loom::Network;

extern "C"
{

NETWORK_API IModule *CreateModule( void )
{
	return new cModuleNetwork();
}

}
