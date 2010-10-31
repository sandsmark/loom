#pragma once

#include <Core/Module/IModule.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Core::IModule;
#include <NetworkManager.h>

#include <windows.h>
BEGIN_NAMESPACE( Network )

class NETWORK_API cModuleNetwork : public IModule, public Speech::ISpeechEvent
{
protected:
	HANDLE mThread;

public:
	cModuleNetwork();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );

	virtual void OnSay( const std::wstring &text );
	virtual void OnHeard( const std::wstring &text );
};

END_NAMESPACE()
