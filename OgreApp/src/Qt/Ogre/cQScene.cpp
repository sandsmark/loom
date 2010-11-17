#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Core/Event/cDispatcherHub.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>

#pragma warning( push )
#pragma warning( disable: 4189 )	// local variable is initialized but not referenced
#include <Ogre/Ogre.h>
#pragma warning( pop )

using namespace Loom::OgreApp;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cOgreResponderOnRender;

/************************************************************************/
cQScene::cQScene( QWidget *iParent )
: cQOgre( iParent )
/************************************************************************/
{
}

/**********************************************************************/
bool cQScene::InitResources( void )
/**********************************************************************/
{
#ifdef Q_WS_MAC
	Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
	Ogre::String vPath = "resources/";
#endif
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "materials/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "fonts/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath, "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); 

	return true;
}

/**********************************************************************/
bool cQScene::InitPost( void )
/**********************************************************************/
{
	// Setup camera
	mCamera->setPosition( Ogre::Vector3( 0, 0, -200 ) );
	mCamera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
	mCamera->setNearClipDistance( 1 );
	mCamera->setFarClipDistance( 1000 );

	// Set some fancy clear color
	Ogre::ColourValue vBGColor; vBGColor.setAsABGR( 0xccaa8844 );
	mViewport->setBackgroundColour( vBGColor );
//	mScene->setFog( Ogre::FOG_LINEAR, vBGColor, 0.001f, 0, 1 );

	return true;
}

/**********************************************************************/
void cQScene::RenderFrame( void )
/**********************************************************************/
{
	cDispatcherHub::Get().Dispatch( cOgreResponderOnRender::Get().GetEventName(), NULL );
	
//    ASSERTTXT( mWindow, "Main window is invalid" );
//    ASSERTTXT( mCamera, "Main camera is invalid" );
    
    update();
}
