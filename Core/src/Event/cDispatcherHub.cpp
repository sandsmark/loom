#include <Core/Event/cDispatcherHub.h>

#include <windows.h>
#include <strsafe.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>

using namespace Loom::Core;
using Loom::Core::cLogger;
using Loom::Core::cLogWriterDebugOutput;

/************************************************************************/
void cDispatcherHub::Register( const cString &iName, IResponder *iResponder )
/************************************************************************/
{	// TODO: Check multiple addition
	mResponders[ iName ].Add( iResponder );
}

/************************************************************************/
void cDispatcherHub::Dispatch( const cString &iName, const IParam &iParam )
/************************************************************************/
{
	cArray<IResponder*> *vList = mResponders.Find( iName );
	if ( !vList ) return;

	// TODO: Make log function with printf
	TCHAR Temp[ 256 ];
	StringCchPrintf( Temp, 256, _T("[cDispatcherHub::Dispatch] Dispatching \"%s\""), iName.ToCString() );
	cLogger::Get().Log( cLogger::LOG_DEBUG, GetLoggerName(), Temp );

	for ( size_t i=0; i<vList->GetSize(); i++ )
	{
		IResponder *vResponder = (*vList)[i];
		vResponder->Respond( iParam );
	}
}

/************************************************************************/
void cDispatcherHub::Init()
/************************************************************************/
{
	cLogger::Get().AddWriter( cString( GetLoggerName() ), new cLogWriterDebugOutput( cLogger::LOG_DEBUG ) );
}
/************************************************************************/
void cDispatcherHub::Destroy()
/************************************************************************/
{
	// TODO: Delete writer
}
