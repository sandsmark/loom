#include <Scripting/Module/cModuleScripting.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <angelscript.h>
#include <Scripting/angelscript/addon/scriptstdstring/scriptstdstring.h>
#include <Scripting/angelscript/addon/scriptany/scriptany.h>
#include <Scripting/angelscript/addon/scriptarray/scriptarray.h>
#include <Scripting/angelscript/addon/scriptbuilder/scriptbuilder.h>
#include <Scripting/Event/Responders/cScriptingResponderRunScript.h>
#include <Scripting/Event/Responders/cScriptingResponderRunDll.h>
#include <Scripting/Event/Responders/cScriptingResponderRunNative.h>
#include <process.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::Scripting;

asIScriptEngine *Loom::Scripting::cModuleScripting::mEngine = NULL;

/************************************************************************/
cModuleScripting::cModuleScripting()
: IModule( _T( "Scripting" ) )
/************************************************************************/
{
}

void MessageCallback(const asSMessageInfo *msg, void * /*param*/)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

/************************************************************************/
void cModuleScripting::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleScripting startup" ) );

	// Initialize angelscript
	mEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
	ASSERTTXT( mEngine, _T( "Can't create scripting engine!" ) );
	mEngine->SetMessageCallback( asFUNCTION( MessageCallback ), 0, asCALL_CDECL );
	RegisterStdString( mEngine );
	RegisterScriptAny( mEngine );
	RegisterScriptArray( mEngine, true );

	int vResult = mEngine->RegisterGlobalFunction( "void Dispatch( string, string )", asFUNCTION( Loom::Scripting::cModuleScripting::OnDispatch ), asCALL_CDECL );
//	int vResult = mEngine->RegisterGlobalFunction( "void Dispatch( string, array<uint8> @ )", asFUNCTION( Loom::Scripting::cModuleScripting::OnDispatch ), asCALL_CDECL );
//	int vResult = mEngine->RegisterGlobalFunction( "void Dispatch( string, any )", asFUNCTION( Loom::Scripting::cModuleScripting::OnDispatch_Generic ), asCALL_GENERIC );
	ASSERTTXT( vResult >= 0, _T( "Can't register Dispatch function!" ) );

	cScriptingResponderRunScript::Get().AddListener( *this );
	cScriptingResponderRunDll::Get().AddListener( *this );
	cScriptingResponderRunNative::Get().AddListener( *this );

	mInitialized = true;
}

/************************************************************************/
void cModuleScripting::Destroy( void )
/************************************************************************/
{
	mEngine->Release();

	mInitialized = false;
}

/************************************************************************/
void Loom::Scripting::cModuleScripting::OnRunScript( const char *iFileName )
/************************************************************************/
{
	int vResult;

	CScriptBuilder vBuilder;
	vResult = vBuilder.StartNewModule( mEngine, "scriptRunner" );
	ASSERTTXT( vResult >= 0, _T( "Can't start new module!" ) );
	vResult = vBuilder.AddSectionFromFile( iFileName );
	ASSERTTXT( vResult >= 0, _T( "Can't load script file!" ) );
	vResult = vBuilder.BuildModule();
	ASSERTTXT( vResult >= 0, _T( "Can't build script!" ) );

	asIScriptModule *vModule = mEngine->GetModule("scriptRunner");
	int vMain = vModule->GetFunctionIdByDecl("void main()");
	ASSERTTXT( vMain >= 0, _T( "Can't find 'void main()' in script!" ) );

	asIScriptContext *vContext = mEngine->CreateContext();
	vResult = vContext->Prepare( vMain );
	ASSERTTXT( vResult >= 0, _T( "Can't prepare script!" ) );

	vResult = vContext->Execute();
	ASSERTTXT( vResult >= 0, _T( "Can't execute script!" ) );

	vContext->Release();
}

/************************************************************************/
void Loom::Scripting::cModuleScripting::OnDispatch( std::string iEventName, std::string iData )
/************************************************************************/
{
//	cDispatcherHub::IParam vIParam( iData );
	const char *vName = iEventName.c_str();
	TCHAR vTemp[ 1024 ];
	mbstowcs_s( NULL, vTemp, 1024, vName, 1023 );
	cDispatcherHub::Get().Dispatch( vTemp, (void*)iData.c_str() );
}

/************************************************************************/
void Loom::Scripting::cModuleScripting::OnDispatch_Generic( asIScriptGeneric *iParams )
/************************************************************************/
{
	void *vParam1 = iParams->GetAddressOfArg(0);
	void *vParam2 = iParams->GetAddressOfArg(1);
	vParam1;
	vParam2;
}

typedef int (*fMain)(int,char**);

/************************************************************************/
void Loom::Scripting::cModuleScripting::OnRunDll( const char *iFileName )
/************************************************************************/
{
	HMODULE vModule = LoadLibraryA( iFileName );
	fMain vMain = (fMain)GetProcAddress( vModule, "main" );
	vMain( 0, NULL );
	FreeLibrary( vModule );
}

/************************************************************************/
void Loom::Scripting::cModuleScripting::OnRunNative( const char *iFileName )
/************************************************************************/
{
//	iFileName;
	char vCommand[ 2048 ];
	sprintf( vCommand, "resources\\\\scripting\\\\cl /LD /MT /I \"..\\\\..\\\\include\" /I \"c:\\\\Program Files\\\\Microsoft Visual Studio 9.0\\\\VC\\\\include\" /I \"c:\\\\Program Files\\\\Microsoft SDKs\\\\Windows\\\\v6.0A\\\\Include\" /D \"_WINDLL\" /D \"_UNICODE\" /D \"UNICODE\" /Fe\"resources\\\\scripting\\\\temp\\\\temp.dll\" /Fo\"resources\\\\scripting\\\\temp\\\\temp.obj\" \"%s\" /link /LIBPATH:\"resources\\\\scripting\\\\lib\" \"..\\\\..\\\\Core\\\\bin\\\\core_d.lib\"", iFileName );
	system( vCommand );

	OnRunDll( "resources\\scripting\\temp\\temp.dll" );
}
