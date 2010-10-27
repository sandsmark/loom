#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>

BEGIN_NAMESPACE( OgreApp )

class cQMainWindow;

class OGREAPP_API cModuleOgreApp : public IModule
{
protected:
	cQMainWindow *mMainWindow;

public:
	cModuleOgreApp();

	virtual void Init( void );
	virtual void Destroy( void );

	static const cString &GetName( void )
	{
		static const cString vName = _T("OgreApp");
		return vName;
	}

	cQMainWindow *GetMainWindow( void ) const { return mMainWindow; }
};

END_NAMESPACE()
