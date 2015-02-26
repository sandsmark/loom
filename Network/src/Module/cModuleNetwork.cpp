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

	Speech::cSpeechResponderHeard::Get().AddListener( *this );
	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	
	mInitialized = true;
}

/************************************************************************/
void cModuleNetwork::OnSay( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}

/************************************************************************/
void cModuleNetwork::OnHeard( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}


/************************************************************************/
DWORD cModuleNetwork::StartThread( LPVOID /*iParam*/ )
/************************************************************************/
{
	// Do listening and routing here
	// Listen for message name and data and send with Dispatch
	for ( ;; )
	{
#if 0		
		// Test
	//	std::wstring text = _T("Hello World");
	//	cDispatcherHub::IParam vReceivedParam( (void*)text.c_str() );

	//	const wchar_t *vReceivedMessageName = _T("Speech::Say");

		// Dispatch received message
	//	cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );
#endif
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
