#include "StdAfx/StdAfx.h"
#include "Avatar.h"
#include "Module/cModuleAvatar.h"

using namespace Loom::Avatar;

extern "C"
{

AVATAR_API IModule *CreateModule( void )
{
	return new cModuleAvatar();
}

}
