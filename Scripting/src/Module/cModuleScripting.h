#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>
#include <Scripting/Event/IScriptingEvent.h>

class asIScriptEngine;
class CScriptAny;
class asIScriptGeneric;

BEGIN_NAMESPACE( Scripting )

class SCRIPTING_API cModuleScripting : public IModule, public IScriptingEvent
{
protected:
	HANDLE mThread;
	static asIScriptEngine *mEngine;

public:
	cModuleScripting();

	virtual void Init( void );
	virtual void Destroy( void );

	static void OnDispatch( std::string iEventName, std::string iData );
	static void OnDispatch_Generic( asIScriptGeneric *iParams );

	// IScriptingEvent methods
	virtual void OnRunDll( const char *iFileName );
	virtual void OnRunNative( const char *iFileName );
	virtual void OnRunScript( const char *iFileName );

};

END_NAMESPACE()
