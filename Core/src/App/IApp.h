#pragma once

BEGIN_NAMESPACE( Core )

class CORE_API IApp
{
protected:
	static IApp *mInstance;

public:
	IApp()
	{
		mInstance = this;
	}

	virtual int Run( int iNumArgs, char *iArgs[] ) = 0;
	virtual const cString &GetName( void ) const = 0;
	virtual const cString &GetOrganization( void ) const = 0;

	static IApp *GetInstance( void ) { return mInstance; }
};

END_NAMESPACE()
