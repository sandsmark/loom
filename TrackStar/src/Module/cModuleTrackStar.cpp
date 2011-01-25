#include <TrackStar/Module/cModuleTrackStar.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <TrackStar/Module/TrackStarInterface.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::TrackStar;

/************************************************************************/
cModuleTrackStar::cModuleTrackStar()
: IModule( GetName() )
/************************************************************************/
{
	tsInterface = NULL;
}

/************************************************************************/
void cModuleTrackStar::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleTrackStar startup" ) );

	Speech::cSpeechResponderHeard::Get().AddListener( *this );
	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	
	tsInterface = new TrackStarDirectReader();
	if (!((TrackStarDirectReader*)tsInterface)->init()) {
		delete(tsInterface);
		tsInterface = NULL;
	}

	mInitialized = true;
}

/************************************************************************/
void cModuleTrackStar::OnSay( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}

/************************************************************************/
void cModuleTrackStar::OnHeard( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}


/************************************************************************/
DWORD cModuleTrackStar::StartThread( LPVOID /*iParam*/ )
/************************************************************************/
{
	//std::wfstream *vStream = new std::wfstream( "myfile.conf" );
	//cSerializerXML *vSerializer = new cSerializerXML( *vStream );
	//MyObject* obj = vSerializer->Deserialize();

	//const cClass &iClass = iObject->GetRTTI();
	//mStream<<iClass.GetName().ToCString()<<":\n";

	// Do listening and routing here
	// Listen for message name and data and send with Dispatch
	for ( ;; )
	{

#if 0		
		// Test
	//	std::wstring text = _T("Hello World");
	//	cDispatcherHub::IParam vReceivedParam( (void*)text.c_str() );

	//	const TCHAR *vReceivedMessageName = _T("Speech::Say");

		// Dispatch received message
	//	cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );
#endif
		Sleep( 5000 );
	}
}

/************************************************************************/
void cModuleTrackStar::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	delete(tsInterface);
	tsInterface = NULL;

	mInitialized = false;
}

/************************************************************************/
TrackStarInterface *Loom::TrackStar::cModuleTrackStar::GetInterface( void )
/************************************************************************/
{
	return tsInterface;
}
