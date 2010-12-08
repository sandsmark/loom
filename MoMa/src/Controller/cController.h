#pragma once

#include <windows.h>
#include <Psyclone\Event\IPsycloneSpeechOnEvent.h>
#include <Psyclone\Event\Responders\cPsycloneSpeechOn.h>
using Loom::Psyclone::IPsycloneSpeechOnEvent;
//using Loom::Speech::ISpeechEvent;

BEGIN_NAMESPACE( MoMa )

class cCreature;

class cController : public IPsycloneSpeechOnEvent
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
	virtual void OnSpeechOn( const std::wstring &text );	
};

END_NAMESPACE()
