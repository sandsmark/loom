#pragma once

#include <Scripting/Event/Responders/IScriptingResponder.h>

BEGIN_NAMESPACE( Scripting )

class cScriptingResponderRunScript : public IScriptingResponder<cScriptingResponderRunScript>
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
	cScriptingResponderRunScript() : IScriptingResponder( _T("Scripting::cScriptingResponderRunScript") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IScriptingEvent::OnRunScript, (const char *)vParam.Path );
	}
};

END_NAMESPACE()