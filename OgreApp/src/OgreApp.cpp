#include <OgreApp/OgreApp.h>
#include <OgreApp/Module/cModuleOgreApp.h>

using namespace Loom::OgreApp;

extern "C"
{

OGREAPP_API IModule *CreateModule( void )
{
	return new cModuleOgreApp();
}

}
