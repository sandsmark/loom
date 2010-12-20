#pragma once

#include <Core/Debug/Logger/Writer/ILogWriter.h>

BEGIN_NAMESPACE( OgreApp )

class cString;

class OGREAPP_API cLogWriterOgreApp : public Core::ILogWriter
{
public:
	cLogWriterOgreApp( const Loom::Core::cLogger::eSeverity iMinSeverity ) : ILogWriter( iMinSeverity ) {}

	virtual void Log( const Loom::Core::cString &iMessage );
};


END_NAMESPACE()
