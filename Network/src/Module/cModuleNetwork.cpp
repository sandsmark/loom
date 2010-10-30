#include <Network/Module/cModuleNetwork.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::Network;

/************************************************************************/
cModuleNetwork::cModuleNetwork()
: IModule( _T( "Network" ) )
/************************************************************************/
{
}

/************************************************************************/
void cModuleNetwork::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleNetwork startup" ) );

	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	
	mInitialized = true;
}

/************************************************************************/
DWORD cModuleNetwork::StartThread( LPVOID /*iParam*/ )
/************************************************************************/
{
	// Do listening and routing here
	// Listen for message name and data and send with Dispatch
	for ( ;; )
	{

		// Test
		char text[] = "Hello World";
		cDispatcherHub::IParam vReceivedParam( (void*)text );

		const TCHAR *vReceivedMessageName = _T("Speech::cSpeechResponderSay");

		// Dispatch received message
		cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );
		Sleep( 5000 );
	}
}

/************************************************************************/
void cModuleNetwork::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}
