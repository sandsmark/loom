#include <Weaver/App/cApp.h>

#include <windows.h>
#include <strsafe.h>
#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>
#include <Core/Event/cDispatcherHub.h>
//#include <Weaver/Qt/MainWindow/cQMainWindow.h>
#include <Core/Module/cModuleManager.h>
#include <iostream>

using namespace Loom::Weaver;
using namespace Loom::Core;

/************************************************************************/
void cApp::Init( void ) 
/************************************************************************/
{
	mInstance = this;

	mConsole = NULL;

	/*
	// Create debug console
	if( AllocConsole() )
	{
		freopen_s( &mConsole, "CONOUT$", "wt", stdout );
		SetConsoleTitleA( "Loom Console" );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
	}
	mOldErr = std::cerr.rdbuf( &mNewOut );
	mOldOut = std::cout.rdbuf( &mNewOut );
	*/

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

	if ( mConsole )
	{
		std::cout.rdbuf( mOldOut );
		std::cerr.rdbuf( mOldErr );
		fclose( mConsole );
	}
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
	