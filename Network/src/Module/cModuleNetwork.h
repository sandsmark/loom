#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>

BEGIN_NAMESPACE( Network )

class NETWORK_API cModuleNetwork : public IModule
{
protected:
	HANDLE mThread;

public:
	cModuleNetwork();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
};

END_NAMESPACE()
