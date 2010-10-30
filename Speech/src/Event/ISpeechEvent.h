#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Speech )

class ISpeechEvent
{
public:
	virtual void OnSay( const char* text ) = 0;
};

END_NAMESPACE()