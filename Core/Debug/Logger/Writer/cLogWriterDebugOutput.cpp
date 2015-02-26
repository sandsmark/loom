#include <Core/Debug/Logger/Writer/cLogWriterDebugOutput.h>
#include <windows.h>

#include <Core/Datatype/cString.h>

using namespace Loom::Core;

/************************************************************************/
void cLogWriterDebugOutput::Log( const cString &iMessage )
/************************************************************************/
{
	OutputDebugString( iMessage.ToCString() );
	OutputDebugString( _T("\n") );
}
