#pragma once

#include <Weaver\Sandbox\ISandboxEvent.h>
#include <Core\Singleton\ISingleton.h>

BEGIN_NAMESPACE( Weaver )

class cSandboxEventListener : public ISandboxEvent
{
public:
	virtual void OnDebugMessage( const cParam &iParam );
};

class cSandboxHub : public IEventDispatcher<ISandboxEvent>, public ISingleton<cSandboxHub>
{
public:
	void Init( void );
	void Destroy( void );
};

END_NAMESPACE()