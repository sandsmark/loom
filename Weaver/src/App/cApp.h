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

//class cQMainWindow;

class cDebugOutput : public std::streambuf
{
public:
	cDebugOutput()
	{
		setp(0, 0);
	}

	virtual int_type overflow( int_type c = traits_type::eof() )
	{
		return fputc(c, stdout) == EOF ? traits_type::eof() : c;
	}
};

class cApp : public Loom::Core::IApp, public QApplication, public Loom::Core::ISingleton<cApp>
{
protected:
//	cQMainWindow *mMainWindow;
	FILE *mConsole;
	std::streambuf *mOldOut;
	std::streambuf *mOldErr;

	cDebugOutput mNewOut;

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
	virtual const Loom::Core::cString &GetOrganization( void ) const
	{
		static Loom::Core::cString vOrganization( _T("CMLabs") );
		return vOrganization;
	}
};

END_NAMESPACE()

