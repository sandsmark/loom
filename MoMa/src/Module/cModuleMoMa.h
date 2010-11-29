#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;
using Loom::Core::cObject;

#include <windows.h>

BEGIN_NAMESPACE( MoMa )

class MOMA_API cModuleMoMa : public IModule
{
protected:
	HANDLE mThread;

public:
	cModuleMoMa();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
};

END_NAMESPACE()
