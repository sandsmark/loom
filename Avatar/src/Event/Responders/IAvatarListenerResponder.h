#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Avatar/Event/IAvatarListenerEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( Avatar )

template< class iClass >
class IAvatarListenerResponder : public IEventDispatcher<IAvatarListenerEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	IAvatarListenerResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
