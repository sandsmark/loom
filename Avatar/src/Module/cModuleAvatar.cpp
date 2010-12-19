#include <Avatar/Module/cModuleAvatar.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Ogre/OgreMath.h>
#include <Ogre/OgreColourValue.h>
#include <Ogre/OgreBillboardSet.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Speech/Speech.h>
#include <Speech/Module/cModuleSpeech.h>

#pragma warning( push )
#pragma warning( disable: 4995 )	// warning C4995: 'vsprintf': name was marked as #pragma deprecated
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Ogre/OgreSceneManager.h>
#pragma warning( pop )

#define _USE_MATH_DEFINES
#undef M_PI
#include <math.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cModuleOgreApp;
using Loom::Speech::cModuleSpeech;

using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQOgre;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;

using namespace Loom::Avatar;

/************************************************************************/
cModuleAvatar::cModuleAvatar()
: IModule( _T( "Avatar" ) )
/************************************************************************/
{
	mDependencies.Add( cModuleOgreApp::GetName() );
	mDependencies.Add( cModuleSpeech::GetName() );
}

/************************************************************************/
void cModuleAvatar::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleAvatar startup" ) );

#ifdef Q_WS_MAC
	Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
	Ogre::String vPath = "resources/";
#endif
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "models/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "textures/", "FileSystem" );

	CreateEnvironment();

 	cOgreAvatar *vAvatar1 = new cOgreAvatar( "Avatar1" );
	vAvatar1;
	cOgreAvatar *vAvatar2 = new cOgreAvatar( "Avatar2" );
	vAvatar2->SetPosition( Ogre::Vector3( 0, 0, -50 ) );
	vAvatar2->SetRotation( Ogre::Quaternion( Ogre::Radian( 0 ), Ogre::Vector3( 0, 1, 0 ) ) );

	mInitialized = true;
}

/************************************************************************/
DWORD cModuleAvatar::StartThread( LPVOID iParam )
/************************************************************************/
{
	while ( true )
	{
		Sleep( 1000 );

		// Test
		struct sTemp
		{
			Ogre::ColourValue Colour;
		};
		sTemp vParam;
		vParam.Colour.r = Ogre::Math::RangeRandom( 0, 1 );
		vParam.Colour.g = Ogre::Math::RangeRandom( 0, 1 );
		vParam.Colour.b = Ogre::Math::RangeRandom( 0, 1 );
		vParam.Colour.a = Ogre::Math::RangeRandom( 0, 1 );
		cDispatcherHub::IParam vIParam( (void*)&vParam );

		cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderSetBGColour"), vIParam );
	}
}

/************************************************************************/
void cModuleAvatar::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}

/************************************************************************/
void Loom::Avatar::cModuleAvatar::CreateEnvironment( void )
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();
	
	vScene->getCamera( "MainCamera" )->setPosition( Ogre::Vector3( 0, 100, -200 ) );
	vScene->getCamera( "MainCamera" )->lookAt( Ogre::Vector3( 0, 0, 0 ) );

	// Create spotlight
	Ogre::Light* vLightSpot1;
	vLightSpot1 = vScene->createLight("SpotLight1");
	vLightSpot1->setType(Ogre::Light::LT_SPOTLIGHT);
	//	vLightSpot1->setPosition(-900, 750, 375);
	vLightSpot1->setDirection(0.79, -0.38, 0.48);
	vLightSpot1->setPosition(-790, 380, -480);
	vLightSpot1->setDiffuseColour(0.5, 0.5, 0.5);
}
