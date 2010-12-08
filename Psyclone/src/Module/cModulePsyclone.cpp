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
	
	plug = new CppAIRPlug("Loom", "localhost", 10000);
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
		std::string s(text.begin(), text.end());
		s.assign(text.begin(), text.end());
		plug->postOutputMessage(new Message("Loom","WB4","RU.S1.Input.Sentence.New",JString(s.c_str()),""));		
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

	const TCHAR *vReceivedMessageName = _T("Psyclone::SpeechOn");

	Message* msg;
	for ( ;; ) {
		if (msg = plug->waitForNewMessage(50)) {
			// Send appropriate event...
			JString from = msg->getFrom();
			JString msgtype = msg->getType().c_str();	
			const char* cm = msgtype.c_str();

			// Convert to a wchar_t*
			size_t origsize = strlen(cm) + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t wcstring[newsize];
			mbstowcs_s(&convertedChars, wcstring, origsize, cm, _TRUNCATE);
			

			cDispatcherHub::IParam vReceivedParam( (void*)wcstring );			
			cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );			
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
