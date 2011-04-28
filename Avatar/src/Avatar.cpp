#include "StdAfx.h"
#include <Avatar/Avatar.h>
#include <Avatar/Module/cModuleAvatar.h>

using namespace Loom::Avatar;

extern "C"
{

AVATAR_API IModule *CreateModule( void )
{
	return new cModuleAvatar();
}

}
