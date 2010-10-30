#include <Speech/Module/cModuleSpeech.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Speech/Event/cSpeechResponders.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::Speech;

/************************************************************************/
cModuleSpeech::cModuleSpeech()
: IModule( _T( "Speech" ) )
/************************************************************************/
{
}

/************************************************************************/
void cModuleSpeech::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleSpeech startup" ) );

	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );

	cSpeechResponderSay::Get().AddListener( *this );

	mInitialized = true;
}


/************************************************************************/
void cModuleSpeech::OnSay( const char* text )
/************************************************************************/
{
	text = text;
}

/************************************************************************/
DWORD cModuleSpeech::StartThread( LPVOID /*iParam*/ )
/************************************************************************/
{
	// Do listening and routing here
	// Listen for message name and data and send with Dispatch
	for ( ;; )
	{
		Sleep( 1000 );

		// Test
		struct sData
		{
			float r,g,b,a;
		};
		sData vParam;
		vParam.r = ( rand() % 1000 ) / 10000.0f;
		vParam.g = ( rand() % 1000 ) / 10000.0f;
		vParam.b = ( rand() % 1000 ) / 10000.0f;
		vParam.a = ( rand() % 1000 ) / 10000.0f;
		cDispatcherHub::IParam vReceivedParam( (void*)&vParam );

		const TCHAR *vReceivedMessageName = _T("Ogre::cOgreResponderSetBGColour");

		// Dispatch received message
		cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );
	}
}

/************************************************************************/
void cModuleSpeech::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}
