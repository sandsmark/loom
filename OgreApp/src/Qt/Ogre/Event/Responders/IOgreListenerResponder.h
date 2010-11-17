#pragma once

#include <Core/Event/cDispatcherHub.h>
#include <Core/Event/IEventDispatcher.h>
#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Core/Singleton/ISingleton.h>

using Loom::Core::IEventDispatcher;
using Loom::Core::ISingleton;
using Loom::Core::cDispatcherHub;

BEGIN_NAMESPACE( OgreApp )

template< class iClass >
class IOgreListenerResponder : public IEventDispatcher<IOgreListenerEvent>, public cDispatcherHub::IResponder, public ISingleton<iClass>
{
public:
	IOgreListenerResponder( const cString &iEventName ) : IResponder( iEventName ) {}
};

END_NAMESPACE()
