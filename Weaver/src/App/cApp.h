#pragma once

#include <Core/App/IApp.h>
#include <Core/Datatype/cString.h>
#include <Core/Singleton/ISingleton.h>
#include <tchar.h>

#pragma warning( push )
#pragma warning( disable: 4512 )	// assignment operator could not be generated
#pragma warning( disable: 4510 )	// default constructor could not be generated
#include <QtGui/QtGui>
#pragma warning( push )

BEGIN_NAMESPACE( Weaver )

class cQMainWindow;

class cApp : public Loom::Core::IApp, public QApplication, public Loom::Core::ISingleton<cApp>
{
protected:
	cQMainWindow *mMainWindow;

	void Init( void );
	void Destroy( void );

public:
	cApp() : QApplication( __argc, __argv ) {}

	// IApp methods
	virtual int Run( int iNumArgs, char *iArgs[] );
	virtual const Loom::Core::cString &GetName( void ) const
	{
		static Loom::Core::cString vName( _T("Weaver") );
		return vName;
	}
};

END_NAMESPACE()

