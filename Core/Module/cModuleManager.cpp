#include <Core/Module/cModuleManager.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>
#include <Core/Module/IModule.h>
#include <strsafe.h>

using namespace Loom::Core;
using Loom::Core::cLogger;
using Loom::Core::cLogWriterDebugOutput;
using Loom::Core::IModule;

/************************************************************************/
void cModuleManager::Init( void )
/************************************************************************/
{
	cLogger::Get().AddWriter( cString( GetLoggerName() ), new cLogWriterDebugOutput( cLogger::LOG_DEBUG ) );

	// First, load all the modules
	WIN32_FIND_DATA vFile;
	HANDLE vHandle;
	SetDllDirectory( _T("Modules") );	// Add modules directory to dll search path
	vHandle = FindFirstFile( _T( "modules/*.dll" ), &vFile );
	BOOL vNext = ( vHandle != INVALID_HANDLE_VALUE );
	while ( vNext )
	{
		cString vPath = _T( "modules/" );
		vPath += vFile.cFileName;
		HMODULE vHModule = LoadLibrary( vPath.ToCString() );
		DWORD vErr = GetLastError();
		vErr;
		fCreateModule vCreateModule = (fCreateModule)GetProcAddress( vHModule, "CreateModule" );
		ASSERTTXT( vCreateModule, _T( "Invalid loom module" ) );

		// TODO: Make log function with printf
		wchar_t Temp[ 256 ];
		StringCchPrintf( Temp, 256, _T("[cModuleManager::Init] \"%s\" module loaded"), vFile.cFileName );
		cLogger::Get().Log( cLogger::LOG_INFO, GetLoggerName(), Temp );

		IModule *vModule = vCreateModule();
		mModules[ vModule->GetName() ] = vModule;

		vNext = FindNextFile( vHandle, &vFile );
	}

	SetDllDirectory( NULL );

	// Then resolve dependencies
	bool vRemaining = false;
	bool vDone = false;
	while ( !vDone )
	{
		vDone = true;
		vRemaining = false;
		for ( cHashMap<const cString,IModule*>::iterator it=mModules.begin(); it != mModules.end(); it++ )
		{
			IModule *vModule = it->second;
			if ( vModule->IsInitialized() ) continue;

			vRemaining = true;
			const cArray<const cString> &vDependencies = vModule->GetDependencies();
			size_t vNumDependencies = vDependencies.size();
			size_t i=0;
			for ( ; i<vNumDependencies; i++ )
			{
				IModule **vDependency = mModules.Find( vDependencies[i] );
				ASSERTTXT( vDependency, _T("Required module is missing!") );
				if ( !(*vDependency)->IsInitialized() ) break;
			}
			// Check if there are still uninitialized depencencies
			if ( i < vNumDependencies ) continue;

			// We can initialize now
			mInitOrder.Add( vModule );
			vModule->Init();
			vDone = false;	// Need more iterations
		}
	}

	ASSERTTXT( !vRemaining, _T("Couldn't resolve all dependencies!" ) );
}

/************************************************************************/
void cModuleManager::Destroy( void )
/************************************************************************/
{
	size_t vNumModules = mInitOrder.size();
	for ( size_t i=vNumModules; i-->0; )
	{
		mInitOrder[i]->Destroy();
	}
}


/************************************************************************/
IModule *cModuleManager::GetModule( const cString &iName )
/************************************************************************/
{
	IModule **vModule = mModules.Find( iName );
	if ( !vModule ) return NULL;

	return *vModule;
}
