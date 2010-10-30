#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>

BEGIN_NAMESPACE( Speech )

class SPEECH_API cModuleSpeech : public IModule
{
protected:
	HANDLE mThread;

public:
	cModuleSpeech();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
};

END_NAMESPACE()
