#pragma once

#include <Core/Container/cHashMap.h>
#include <Core/Datatype/cString.h>
#include <Core/Singleton/ISingleton.h>

BEGIN_NAMESPACE( Core )

class ILogWriter;

class CORE_API cLogger : public ISingleton<cLogger>
{
	friend class ISingleton<cLogger>;

public:
	enum eSeverity
	{
		LOG_NONE    =   0,
		LOG_ERROR   =  50,
		LOG_WARNING = 100,
		LOG_INFO    = 150,
		LOG_DEBUG   = 200,
	};

protected:
	cHashMap<const cString,ILogWriter*> mWriters;

protected:
	cLogger();

public:
	void AddWriter( const cString &iName, ILogWriter *iWriter );
	void Log( const eSeverity iSeverity, const cString &iName, const cString &iMessage );

	// cSingleton methods
	void Init( const eSeverity iDefaultLogLevel );
	void Destroy( void );
};

END_NAMESPACE()