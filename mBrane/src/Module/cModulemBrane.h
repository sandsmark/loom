#pragma once

#include <node.h>
#include <Core/Module/IModule.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Core::IModule;

#include <windows.h>
#include <queue>

using	namespace	core;
using	namespace	mBrane;

#ifdef WINDOWS
	#define DllExport   __declspec( dllexport )
#else // WINDOWS
	#define DllExport
#endif // WINDOWS

// API for external code
extern "C" {
	DllExport char* WaitForEvent(uint32 timeout);
	DllExport bool ProcessEvent(char* data);
}

// API for internal code
char* WaitForIncomingEvent(uint32 timeout);
bool OutputEvent(char* data);


BEGIN_NAMESPACE( MBrane )

class MBRANE_API cModulemBrane : public IModule, public Speech::ISpeechListenerEvent
{
protected:
	HANDLE mThread;

public:
	cModulemBrane();
	~cModulemBrane();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );

	virtual void OnSay( const std::wstring &text );
	virtual void OnHeard( const std::wstring &text );

	mBrane::Node *node;
};

END_NAMESPACE()
