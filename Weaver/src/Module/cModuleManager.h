#pragma once

#include <Core/Singleton/ISingleton.h>
#include <Core/Datatype/cString.h>

using Loom::Core::ISingleton;
using Loom::Core::cString;

BEGIN_NAMESPACE( Weaver )

class IModule;

class cModuleManager : public ISingleton<cModuleManager>
{
public:
	void Init( void );
	void Destroy( void );

	static const cString &GetLoggerName()
	{
		static const cString vLogger( _T("ModuleManager") );
		return vLogger;
	}
};

typedef IModule* (*fCreateModule)(void);

END_NAMESPACE()
