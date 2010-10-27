#pragma once

#include <Core/Container/cArray.h>
#include <Core/Datatype/cString.h>

using Loom::Core::cArray;
using Loom::Core::cString;

BEGIN_NAMESPACE( Core )

class CORE_API IModule
{
protected:
	cArray<const cString> mDependencies;
	cString mName;
	bool mInitialized;

protected:
	IModule( const cString &iName ) : mName( iName ), mInitialized( false ) {}

public:

	virtual void Init( void ) = 0;
	virtual void Destroy( void ) = 0;
	const cArray<const cString> &GetDependencies( void ) const { return mDependencies; }
	const cString &GetName( void ) const { return mName; }
	const bool IsInitialized( void ) const { return mInitialized; }
};

END_NAMESPACE()
