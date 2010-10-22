#pragma once

BEGIN_NAMESPACE( Weaver )

class IModule
{
public:
	virtual void Init( void ) = 0;
};

END_NAMESPACE()
