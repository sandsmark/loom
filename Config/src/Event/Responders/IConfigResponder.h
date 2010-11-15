#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Config/Event/IConfigEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( Config )

template< class iClass >
class IConfigResponder : public IEventDispatcher<IConfigEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	IConfigResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
