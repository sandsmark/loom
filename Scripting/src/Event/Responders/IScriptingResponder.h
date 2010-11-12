#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Scripting/Event/IScriptingEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( Scripting )

template< class iClass >
class IScriptingResponder : public IEventDispatcher<IScriptingEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	IScriptingResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
