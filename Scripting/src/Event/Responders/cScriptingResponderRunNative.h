#pragma once

#include <Scripting/Event/Responders/IScriptingResponder.h>

BEGIN_NAMESPACE( Scripting )

class cScriptingResponderRunNative : public IScriptingResponder<cScriptingResponderRunNative>
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
	cScriptingResponderRunNative() : IScriptingResponder( _T("Scripting::cScriptingResponderRunNative") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IScriptingEvent::OnRunNative, (const char *)vParam.Path );
	}
};

END_NAMESPACE()