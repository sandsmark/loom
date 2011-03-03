#pragma once
#include <Core\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Psyclone )

class IDialogueStatusEvent
{
public:
	virtual void DialogueStatus( const std::wstring &text ) = 0;
};

END_NAMESPACE()