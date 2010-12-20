#pragma once

#include <Core/Module/IModule.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Core::IModule;

#include <windows.h>
BEGIN_NAMESPACE( TrackStar )

class TRACKSTAR_API cModuleTrackStar : public IModule, public Speech::ISpeechListenerEvent
{
protected:
	HANDLE mThread;

public:
	cModuleTrackStar();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );

	virtual void OnSay( const std::wstring &text );
	virtual void OnHeard( const std::wstring &text );
};

END_NAMESPACE()
