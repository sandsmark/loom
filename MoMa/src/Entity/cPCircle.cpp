#include <MoMa/Entity/cPCircle.h>
#include <typeinfo>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreSceneManager.h>
#include <MoMa/Entity/cCircle.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>

using namespace Loom::MoMa;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;

/************************************************************************/
cPCircle::cPCircle()
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	mBillboardSet = vScene->createBillboardSet( "MoMa.Circle" );
	mBillboardSet->setMaterialName( "MoMa.Circle" );
	mBillboardSet->setDefaultDimensions( 4, 1 );
	mBillboardSet->setPointRenderingEnabled( false );
	mBillboardSet->setAutoextend( true );
	mBillboardSet->setAutoUpdate( true );
	mBillboardSet->setVisible( true );
	mBillboardSet->setBillboardRotationType( Ogre::BBR_VERTEX );
	vScene->getRootSceneNode()->attachObject( mBillboardSet );

}

/************************************************************************/
cPCircle::~cPCircle()
/************************************************************************/
{
	delete mBillboardSet;
}

/************************************************************************/
cCircle *cPCircle::CreateInstance( void )
/************************************************************************/
{
	cCircle *vCircle = new cCircle( this );

	return vCircle;
}
