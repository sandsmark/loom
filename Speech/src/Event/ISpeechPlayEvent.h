#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Speech )

class ISpeechPlayEvent
{
public:
	virtual void OnSpeechPlay( ) = 0;
};

END_NAMESPACE()