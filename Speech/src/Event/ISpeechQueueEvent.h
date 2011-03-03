#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Speech )

class ISpeechQueueEvent
{
public:
	virtual void OnSpeechQueue( const std::wstring &text ) = 0;
	
};

END_NAMESPACE()