#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>

class QSettings;

BEGIN_NAMESPACE( Config )

class CONFIG_API cModuleConfig : public IModule
{
protected:
	HANDLE mThread;
	QSettings *mSettings;

public:
	cModuleConfig();

	virtual void Init( void );
	virtual void Destroy( void );

	template< typename T >
	void  SetValue( const cString &iKey, const T iValue, const cString *iGroup = NULL );

	template< typename T >
	const void *GetValue( const cString &iKey, const T iDefault, const cString *iGroup = NULL );
};

END_NAMESPACE()
