#pragma once

#include <Core/Datatype/cString.h>
using Loom::Core::cString;

BEGIN_NAMESPACE( Scripting )

class IScriptingEvent
{
public:
	virtual void OnRunScript( const char *iFileName ) = 0;
	virtual void OnRunDll( const char *iFileName ) = 0;
	virtual void OnRunNative( const char *iFileName ) = 0;
};

END_NAMESPACE()
