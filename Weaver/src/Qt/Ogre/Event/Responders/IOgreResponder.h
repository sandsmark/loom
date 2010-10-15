#pragma once

#include <Weaver/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <Weaver/Qt/Ogre/Event/IOgreEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;

BEGIN_NAMESPACE( Weaver )

template< class iClass >
class IOgreResponder : public IEventDispatcher<IOgreEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	IOgreResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
