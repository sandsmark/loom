#include <Weaver/Module/cModuleManager.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>
#include <Weaver/Module/IModule.h>
#include <strsafe.h>

using namespace Loom::Weaver;
using Loom::Core::cLogger;
using Loom::Core::cLogWriterDebugOutput;

/************************************************************************/
void cModuleManager::Init( void )
/************************************************************************/
{
	cLogger::Get().AddWriter( cString( GetLoggerName() ), new cLogWriterDebugOutput( cLogger::LOG_DEBUG ) );

	WIN32_FIND_DATA vFile;
	HANDLE vHandle;
	vHandle = FindFirstFile( _T( "modules/*.dll" ), &vFile );
	BOOL vNext = ( vHandle != INVALID_HANDLE_VALUE );
	while ( vNext )
	{
		cString vPath = _T( "modules/" );
		vPath += vFile.cFileName;
		HMODULE vHModule = LoadLibrary( vPath.ToCString() );
		fCreateModule vCreateModule = (fCreateModule)GetProcAddress( vHModule, "CreateModule" );
		ASSERTTXT( vCreateModule, _T( "Invalid weaver module" ) );

		// TODO: Make log function with printf
		TCHAR Temp[ 256 ];
		StringCchPrintf( Temp, 256, _T("[cModuleManager::Init] \"%s\" module loaded"), vFile.cFileName );
		cLogger::Get().Log( cLogger::LOG_INFO, GetLoggerName(), Temp );

		IModule *Module = vCreateModule();
		Module->Init();

		vNext = FindNextFile( vHandle, &vFile );
	}
}

/************************************************************************/
void cModuleManager::Destroy( void )
/************************************************************************/
{
	// TODO: Delete writer
}

