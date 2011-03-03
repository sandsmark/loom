#pragma once

#include <windows.h>
#include <Ogre/OgreVector3.h>
#include <Psyclone\Event\IPsycloneSpeechOnEvent.h>
#include <Psyclone\Event\IDialogueStatusEvent.h>
#include <Psyclone\Event\Responders\cPsycloneSpeechOn.h>
#include <Psyclone\Event\Responders\cDialogueStatus.h>
using Loom::Psyclone::IPsycloneSpeechOnEvent;
using Loom::Psyclone::IDialogueStatusEvent;
//using Loom::Speech::ISpeechEvent;

BEGIN_NAMESPACE( MoMa )

class cCreature;
class cCircle;

class cController : public IPsycloneSpeechOnEvent, public IDialogueStatusEvent
{
protected:
	typedef enum {
		SPEECH_ON,
		SPEECH_OFF
	}SpeechState;

	HANDLE mThread;
	cArray<cCreature*> mCreatures;
	cArray<cCircle*> mCircles;

	DWORD mLastUpdate;
	float mNextUpdate;
	SpeechState mSpeechState;
protected:
	void CreateWaveform();
	void CreateCircles();
	void CreateCreatures();

	void Init( void );
	void Destroy( void );

	void Update( void );

	double no1(double x);
	static DWORD WINAPI StartThread( LPVOID iParam );
	bool bCharacterTurn;
	bool bUserTurn;
	bool bDoBlink;
	bool bDoEyeGrow;
	DWORD lMoveEyeTimer;
	DWORD lLookAwaytimer;
	Ogre::Vector3 vPrevEyeTarget;

	double x;	
	double y;
	double a1;
	double b1;
	double c1;
	double d1;
	double e1;
	double f1;

	double a2;
	double b2;
	double c2;
	double d2;
	double e2;
	double f2;

	int threshold;

public:
	cController();
	~cController();

	void Run( void );

	// ISpeechListenerEvent
	virtual void OnSpeechOn( const std::wstring &text );	
	virtual void DialogueStatus( const std::wstring &text );	
};

END_NAMESPACE()
