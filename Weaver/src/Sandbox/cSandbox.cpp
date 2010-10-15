#include <Weaver/Sandbox/cSandbox.h>

#include <strsafe.h>

using namespace Loom::Weaver;
using namespace Loom::Core;

/************************************************************************/
void cSandboxEventListener::OnDebugMessage( const cParam &iParam )
/************************************************************************/
{
	TCHAR vTemp[ 256 ];
	StringCchPrintf( vTemp, 256, _T("%s %d"), iParam.mMessage, iParam.mNumber );
	cLogger::Get().Log( cLogger::LOG_DEBUG, _T("Gecike"), vTemp );
}

/************************************************************************/
void cSandboxHub::Init( void )
/************************************************************************/
{
	cDispatcherHub::Get().Register<ISandboxEvent::cParam>( _T("DebugMessage"), &ISandboxEvent::OnDebugMessage, this );
}

/************************************************************************/
void cSandboxHub::Destroy( void )
/************************************************************************/
{
}
