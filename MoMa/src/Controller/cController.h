#pragma once

#include <windows.h>
#include <Ogre/OgreVector3.h>
#include <Psyclone\Event\IPsycloneSpeechOnEvent.h>
#include <Psyclone\Event\Responders\cPsycloneSpeechOn.h>
using Loom::Psyclone::IPsycloneSpeechOnEvent;
//using Loom::Speech::ISpeechEvent;

BEGIN_NAMESPACE( MoMa )

class cCreature;
class cCircle;

class cController : public IPsycloneSpeechOnEvent
{
protected:
	HANDLE mThread;
	cArray<cCreature*> mCreatures;
	cArray<cCircle*> mCircles;

	DWORD mLastUpdate;
	float mNextUpdate;

protected:
	void CreateWaveform();
	void CreateCircles();
	void CreateCreatures();

	void Init( void );
	void Destroy( void );

	void Update( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
	bool bCharacterTurn;
	bool bUserTurn;
	bool bDoBlink;
	bool bDoEyeGrow;
	DWORD lMoveEyeTimer;
	DWORD lLookAwaytimer;
	Ogre::Vector3 vPrevEyeTarget;
public:
	cController();
	~cController();

	void Run( void );

	// ISpeechListenerEvent
	virtual void OnSpeechOn( const std::wstring &text );	
};

END_NAMESPACE()
