#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>

BEGIN_NAMESPACE( Avatar )

class AVATAR_API cModuleAvatar : public IModule
{
protected:
	HANDLE mThread;

	void CreateEnvironment( void );

public:
	cModuleAvatar();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
};

END_NAMESPACE()
