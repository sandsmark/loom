#include <Weaver/App/cApp.h>

#include <windows.h>
#include <strsafe.h>
#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>
#include <Core/Event/cDispatcherHub.h>
//#include <Weaver/Qt/MainWindow/cQMainWindow.h>
#include <Core/Module/cModuleManager.h>

using namespace Loom::Weaver;
using namespace Loom::Core;

/************************************************************************/
void cApp::Init( void ) 
/************************************************************************/
{
	cLogger::Get().Init( cLogger::LOG_DEBUG );
	cDispatcherHub::Get().Init();
	cModuleManager::Get().Init();

//	mMainWindow = new cQMainWindow();
}

/************************************************************************/
void cApp::Destroy( void ) 
/************************************************************************/
{
//	SAFE_DELETE( mMainWindow );

	cModuleManager::Get().Destroy();
	cDispatcherHub::Get().Destroy();
	cLogger::Get().Destroy();
}

/************************************************************************/
int cApp::Run( int /*iNumArgs*/, char*[] /*iArgs[]*/ )
/************************************************************************/
{
	Init();

//	mMainWindow->show();
	exec();

	Destroy();

	return 0;
}
	