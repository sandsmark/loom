#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Speech )

class ISpeechEvent
{
public:
	virtual void OnSay( const std::wstring &text ) = 0;
	virtual void OnHeard( const std::wstring &text ) = 0;
};

END_NAMESPACE()