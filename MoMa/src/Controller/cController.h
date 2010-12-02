#pragma once

#include <windows.h>
#include <Speech/Event/ISpeechListenerEvent.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Speech::ISpeechListenerEvent;

BEGIN_NAMESPACE( MoMa )

class cCreature;

class cController : public ISpeechListenerEvent
{
protected:
	HANDLE mThread;
	cArray<cCreature*> mCreatures;

protected:
	void CreateWaveform();
	void CreateCircles();
	void CreateCreatures();

	void Init( void );
	void Destroy( void );

	void Update( void );

	static DWORD WINAPI StartThread( LPVOID iParam );

public:
	cController();
	~cController();

	void Run( void );

	// ISpeechListenerEvent
	virtual void OnHeard( const std::wstring &text );
};

END_NAMESPACE()
