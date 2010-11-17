#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( OgreApp )

class IOgreListenerEvent
{
public:
	virtual void OnRender( void ) = 0;
};

END_NAMESPACE()