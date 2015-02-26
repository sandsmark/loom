#pragma once

#include <Core/Debug/Logger/Writer/ILogWriter.h>

BEGIN_NAMESPACE( Core )

class cString;

class CORE_API cLogWriterDebugOutput : public ILogWriter
{
public:
	cLogWriterDebugOutput( const cLogger::eSeverity iMinSeverity ) : ILogWriter( iMinSeverity ) {}

	virtual void Log( const cString &iMessage );
};


END_NAMESPACE()
