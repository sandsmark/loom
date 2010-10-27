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
void cModuleNetwork::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}
