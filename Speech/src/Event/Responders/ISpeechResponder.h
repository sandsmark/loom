#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Speech/Event/ISpeechEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( Speech )

template< class iClass >
class ISpeechResponder : public IEventDispatcher<ISpeechEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	ISpeechResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
