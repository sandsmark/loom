#pragma once

#include <Weaver/Module/IModule.h>
using Loom::Weaver::IModule;

#include <windows.h>

BEGIN_NAMESPACE( MoMa )

class cModuleMoMa : public IModule
{
public:
	virtual void Init( void );
	static DWORD WINAPI StartThread( LPVOID iParam );
};

END_NAMESPACE()
