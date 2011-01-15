#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>

BEGIN_NAMESPACE( Avatar )

class cConfig;

class cOgreAvatar;
class AVATAR_API cModuleAvatar : public IModule
{
protected:
	HANDLE mThread;
	cConfig *mConfig;

	cOgreAvatar *mAvatar1, *mAvatar2;

	void CreateEnvironment( void );

public:
	cModuleAvatar();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
};

END_NAMESPACE()
