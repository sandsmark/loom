#pragma once

BEGIN_NAMESPACE( Core )

class cClass;

class IObject
{
protected:
	const cClass *mClass;

public:
	IObject( const cClass *iClass ) : mClass( iClass ) {}

	const cClass &GetRTTI( void ) const { return *mClass; }
};

END_NAMESPACE()
