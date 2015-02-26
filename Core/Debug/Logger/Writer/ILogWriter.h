#pragma once

#include <windows.h>
#include <Core/Debug/Logger/cLogger.h>

BEGIN_NAMESPACE( Core )

class cString;

class CORE_API ILogWriter
{
protected:
	cLogger::eSeverity mMinSeverity;

public:
	ILogWriter( const cLogger::eSeverity iMinSeverity ) : mMinSeverity( iMinSeverity ) {}
	virtual void Log( const cString &iMessage ) = 0;

	const cLogger::eSeverity GetMinSeverity( void ) const { return mMinSeverity; }
	void SetMinSeverity( const cLogger::eSeverity iValue ) { mMinSeverity = iValue; }
};

END_NAMESPACE()
