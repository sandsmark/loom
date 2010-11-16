#include <mBrane/Module/cModulemBrane.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
// #include	"../Core/payload_utils.h"

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

SharedLibrary	SL;	//	will be initialized with the library loaded by the node; this ensures SL is deleted last, and thus that all user-defined adresses (e.g. __vfptr) are still valid until the very end

using namespace Loom::MBrane;

/************************************************************************/
cModulemBrane::cModulemBrane()
: IModule( _T( "mBrane" ) )
/************************************************************************/
{
	node = NULL;
}

/************************************************************************/
cModulemBrane::~cModulemBrane()
/************************************************************************/
{
	if (node) {
		node->shutdown();
		delete	node;
		node = NULL;
	}
}

/************************************************************************/
void cModulemBrane::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModulemBrane startup" ) );

	Speech::cSpeechResponderHeard::Get().AddListener( *this );
	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	
	mInitialized = true;
}

/************************************************************************/
void cModulemBrane::OnSay( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}

/************************************************************************/
void cModulemBrane::OnHeard( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}


/************************************************************************/
DWORD cModulemBrane::StartThread( LPVOID arg )
/************************************************************************/
{
	if (!arg)
		return 0;

	cModulemBrane *_this = (cModulemBrane*) arg;

	LoadControlMessageMetaData();
	Time::Init(1000);

	char configfile[] = "mBrane.xml";
	int verboseLevel = 7;

	_this->node=mBrane::Node::New(configfile,SL,verboseLevel);
	//_this->node=mBrane::Node::NewXML(configXML,SL,verboseLevel);

	// We could not initialise everything, bailing out
	if (!_this->node){
		return 1;
	}

	std::wstring text = _T("mBrane is running...");
	cDispatcherHub::IParam vReceivedParam( (void*)text.c_str() );
	const TCHAR *vReceivedMessageName = _T("Speech::Say");
	cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );

	// std::cout<<"Running\n";

	_this->node->run();

	_this->node->shutdown();
	delete	_this->node;
	_this->node = NULL;

	return 0;
}

/************************************************************************/
void cModulemBrane::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}
