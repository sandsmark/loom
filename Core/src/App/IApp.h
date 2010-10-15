#pragma once

BEGIN_NAMESPACE( Core )

class IApp
{
public:
	virtual int Run( int iNumArgs, char *iArgs[] ) = 0;
	virtual const cString &GetName( void ) const = 0;
};

END_NAMESPACE()
