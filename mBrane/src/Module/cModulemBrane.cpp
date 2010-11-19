#include <mBrane/Module/cModulemBrane.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
// #include	"../Core/payload_utils.h"

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

SharedLibrary	SL;	//	will be initialized with the library loaded by the node; this ensures SL is deleted last, and thus that all user-defined adresses (e.g. __vfptr) are still valid until the very end

using namespace Loom::MBrane;

static core::Mutex inQMutex;
static core::Mutex outQMutex;
static core::Semaphore inQSem(0, 10);
static core::Semaphore outQSem(0, 10);

static std::queue<char*> inQ;
static std::queue<char*> outQ;

static char* test = NULL;

// *****************************
// API for external code
// *****************************
char* WaitForEvent(uint32 timeout) {
	if (!outQMutex.acquire(timeout))
		return NULL;
	if (outQ.size() == 0) {
		// wait for semaphore
		outQMutex.release();
		if (!outQSem.acquire(timeout))
			return NULL;
		if (!outQMutex.acquire(timeout))
			return NULL;
		if (outQ.size() == 0) {
			outQMutex.release();
			return NULL;
		}
	}
	char* data = outQ.front();
	outQ.pop();
	outQMutex.release();
	return data;
}

bool ProcessEvent(char* data) {
	if (!inQMutex.acquire(100))
		return false;
	inQ.push(data);
	inQSem.release();
	inQMutex.release();
	return true;
}


// *****************************
// API for internal code
// *****************************
char* WaitForIncomingEvent(uint32 timeout) {
	if (!inQMutex.acquire(timeout))
		return NULL;
	if (inQ.size() == 0) {
		// wait for semaphore
		inQMutex.release();
		if (!inQSem.acquire(timeout))
			return NULL;
		if (!inQMutex.acquire(timeout))
			return NULL;
		if (inQ.size() == 0) {
			inQMutex.release();
			return NULL;
		}
	}
	char* data = inQ.front();
	inQ.pop();
	inQMutex.release();
	return data;
}

bool OutputEvent(char* data) {
	if (!outQMutex.acquire(100))
		return false;
	outQ.push(data);
	outQSem.release();
	outQMutex.release();
	return true;
}




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
