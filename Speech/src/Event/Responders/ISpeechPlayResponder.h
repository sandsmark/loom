#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Speech/Event/ISpeechPlayEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( Speech )

template< class iClass >
class ISpeechPlayResponder : public IEventDispatcher<ISpeechPlayEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	ISpeechPlayResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
