#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Psyclone/Event/IPsycloneSpeechOnEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( Psyclone )

template< class iClass >
class IPsycloneSpeechOnResponder : public IEventDispatcher<IPsycloneSpeechOnEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	IPsycloneSpeechOnResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
