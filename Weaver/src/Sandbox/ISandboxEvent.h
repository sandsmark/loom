#pragma once
#include <Weaver\Event\cDispatcherHub.h>

BEGIN_NAMESPACE( Weaver )

class ISandboxEvent
{
public:
	class cParam : public cDispatcherHub::IEventParam
	{
	public:
		const cString &mMessage;
		int mNumber;

		cParam( const cString &iMessage, int iNumber )
		: mMessage( iMessage ), mNumber( iNumber )
		{
		}

		cParam( const cParam &iOther )
		: mMessage( iOther.mMessage ), mNumber( iOther.mNumber )
		{
		}

		cParam &operator =( const cParam & )
		{
			throw std::exception( "ISandboxEvent::cParam cannot be copied!" );
		}
	};

public:
	virtual void OnDebugMessage( const cParam &iParam ) = 0;
};

END_NAMESPACE()