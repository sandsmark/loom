#pragma once

#include <mBrane.h>
#include <Core/Module/IModule.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Core::IModule;

#include <windows.h>

using	namespace	core;
using	namespace	mBrane;

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
