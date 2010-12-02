#include <MoMa/Module/cModuleMoMa.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <MoMa/Controller/cController.h>
#include <strsafe.h>
#include <sstream>

using namespace Loom::MoMa;

using Loom::OgreApp::cModuleOgreApp;
using Loom::Core::cLogger;

/************************************************************************/
cModuleMoMa::cModuleMoMa()
: IModule( _T( "MoMa" ) )
/************************************************************************/
{
	mDependencies.Add( cModuleOgreApp::GetName() );
	mController = new cController();
}

/************************************************************************/
void cModuleMoMa::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleMoMa startup" ) );
	
	mController->Run();

	mInitialized = true;
}

/************************************************************************/
void cModuleMoMa::Destroy( void )
/************************************************************************/
{
	delete mController;

	mInitialized = false;
}
