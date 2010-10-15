#include <Core/Debug/Logger/cLogger.h>

#include <Core/Debug/Logger/Writer/ILogWriter.h>
#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>
#include <tchar.h>

using namespace Loom::Core;

/************************************************************************/
void cLogger::AddWriter( const cString &iName, ILogWriter *iWriter )
/************************************************************************/
{
	if ( mWriters.HasKey( iName ) ) return;

	mWriters[ iName ] = iWriter;
}

/************************************************************************/
void cLogger::Log( const eSeverity iSeverity, const cString &iName, const cString &iMessage )
/************************************************************************/
{
	ASSERTTXT( mWriters.HasKey( iName ), _T("Invalid logger") );

	ILogWriter *vWriter = mWriters[ iName ];
	if ( iSeverity > vWriter->GetMinSeverity() ) return;
	mWriters[ iName ]->Log( iMessage );
}

/************************************************************************/
cLogger::cLogger()
/************************************************************************/
{
}

/************************************************************************/
void cLogger::Init( const eSeverity iDefaultLogLevel )
/************************************************************************/
{
	AddWriter( cString( _T("Global") ), new cLogWriterDebugOutput( iDefaultLogLevel ) );
}

/************************************************************************/
void cLogger::Destroy( void )
/************************************************************************/
{
	ILogWriter *vWriter = mWriters[ _T("Global") ];
	mWriters.erase( _T("Global") );
	delete vWriter;
}
