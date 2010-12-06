#include <Psyclone/Module/cModulePsyclone.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
// #include	"../Core/payload_utils.h"

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::Psyclone;

/************************************************************************/
cModulePsyclone::cModulePsyclone()
: IModule( _T( "Psyclone" ) )
/************************************************************************/
{
	plug = NULL;
}

/************************************************************************/
cModulePsyclone::~cModulePsyclone()
/************************************************************************/
{
	if (plug) {
		delete plug;
		plug = NULL;
	}
}

/************************************************************************/
void cModulePsyclone::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModulePsyclone startup" ) );

	Speech::cSpeechResponderHeard::Get().AddListener( *this );

	plug = new CppAIRPlug("MyName", "localhost", 10000);
	if (!plug->init()) {
		printf("Could not connect to Psyclone...\n");
		delete(plug);
		plug = NULL;
	}

	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	
	mInitialized = true;
}

/************************************************************************/
void cModulePsyclone::OnSay( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}

/************************************************************************/
void cModulePsyclone::OnHeard( const std::wstring &text )
/************************************************************************/
{
	text.begin();
	if (plug) {
		// Send message to Psyclone
	}
}


/************************************************************************/
DWORD cModulePsyclone::StartThread( LPVOID arg )
/************************************************************************/
{
	if (!arg)
		return 0;

	cModulePsyclone *_this = (cModulePsyclone*) arg;

	CppAIRPlug* plug = _this->plug;
	if (!plug)
		return 0;

	Message* msg;
	for ( ;; ) {
		if (msg = plug->waitForNewMessage(50)) {
			// Send appropriate event...
		}
	}
	
	return 0;
}

/************************************************************************/
void cModulePsyclone::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}
