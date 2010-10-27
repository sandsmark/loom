#pragma once

#include <Core/Singleton/ISingleton.h>
#include <Core/Datatype/cString.h>
#include <Core/Container/cHashMap.h>
#include <Core/Container/cArray.h>

using Loom::Core::ISingleton;
using Loom::Core::cString;
using Loom::Core::cHashMap;
using Loom::Core::cArray;

BEGIN_NAMESPACE( Core )

class IModule;

class CORE_API cModuleManager : public ISingleton<cModuleManager>
{
protected:
	cHashMap<const cString,IModule*> mModules;
	cArray<IModule*> mInitOrder;

public:
	void Init( void );
	void Destroy( void );

	static const cString &GetLoggerName()
	{
		static const cString vLogger( _T("ModuleManager") );
		return vLogger;
	}

	IModule *GetModule( const cString &iName );
};

typedef IModule* (*fCreateModule)(void);

END_NAMESPACE()
