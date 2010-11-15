#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Speech )

class ISpeechListenerEvent
{
public:
	virtual void OnHeard( const std::wstring &text ) = 0;
};

END_NAMESPACE()