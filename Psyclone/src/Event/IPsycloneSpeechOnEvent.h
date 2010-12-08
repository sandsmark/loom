#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Psyclone )

class IPsycloneSpeechOnEvent
{
public:
	virtual void OnSpeechOn( const std::wstring &text ) = 0;
};

END_NAMESPACE()