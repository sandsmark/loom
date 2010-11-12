#pragma once

#include <Scripting/Event/Responders/IScriptingResponder.h>

BEGIN_NAMESPACE( Scripting )

class cScriptingResponderRunDll : public IScriptingResponder<cScriptingResponderRunDll>
{
public:
	class cParam
	{
	public:
		char *Path;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *iName = ((char*)iParam.GetData());
			Path = new char[ strlen( iName ) + 1 ];
			strcpy( Path, iName );
		}
	};

public:
	cScriptingResponderRunDll() : IScriptingResponder( _T("Scripting::cScriptingResponderRunDll") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IScriptingEvent::OnRunDll, (const char *)vParam.Path );
	}
};

END_NAMESPACE()