#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>

#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreSceneManager.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
using Loom::OgreApp::cQScene;

using namespace Loom::OgreApp;

/************************************************************************/
cModuleOgreApp::cModuleOgreApp()
: IModule( cModuleOgreApp::GetName() ), mMainWindow( NULL )
/************************************************************************/
{
}

/************************************************************************/
void cModuleOgreApp::Init( void )
/************************************************************************/
{
	int a = 0;
	a;

	mMainWindow = new cQMainWindow();
	mMainWindow->show();

	mInitialized = true;
}

/************************************************************************/
void cModuleOgreApp::Destroy( void )
/************************************************************************/
{
	SAFE_DELETE( mMainWindow );
	mInitialized = false;
}

#if 0
/************************************************************************/
const cString &cModuleOgreApp::GetName( void )
/************************************************************************/
{
	static const cString vName = _T("OgreApp");
	return vName;
}
#endif