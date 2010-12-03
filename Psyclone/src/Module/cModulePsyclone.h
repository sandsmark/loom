#pragma once

#include "CppAIRPlug.h"
#include <Core/Module/IModule.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Core::IModule;

#include <windows.h>

using	namespace	cmlabs;

BEGIN_NAMESPACE( Psyclone )

class PSYCLONE_API cModulePsyclone : public IModule, public Speech::ISpeechListenerEvent
{
protected:
	HANDLE mThread;
	CppAIRPlug* plug;

public:
	cModulePsyclone();
	~cModulePsyclone();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );

	virtual void OnSay( const std::wstring &text );
	virtual void OnHeard( const std::wstring &text );
};

END_NAMESPACE()
