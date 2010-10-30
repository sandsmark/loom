#pragma once

#include <Core/Module/IModule.h>
#include <Speech/Event/ISpeechEvent.h>
using Loom::Core::IModule;

#include <windows.h>

BEGIN_NAMESPACE( Speech )

class SPEECH_API cModuleSpeech : public IModule, public ISpeechEvent
{
protected:
	HANDLE mThread;

public:
	cModuleSpeech();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
	
	virtual void OnSay( const char* text );
};

END_NAMESPACE()
