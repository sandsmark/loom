#include "StdAfx.h"
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
#include <Avatar/Controller/cControllerTest.h>
#include <Avatar/Controller/cControllerTest2.h>
#include <Avatar/Controller/cControllerTrack.h>
#include <Avatar/Controller/cControllerInternal.h>
#include <Avatar/Controller/cControllerMouse.h>
#include <Avatar/Controller/cControllerNetwork.h>
#include <Core/Serializer/cSerializerXML.h>
#include <Avatar/Config/cConfig.h>
#include <TrackStar/TrackStar.h>
#include <TrackStar/Module/cModuleTrackStar.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderCreateBox.h>
#include <Avatar/Event/Responders/cAvatarResponderAttach.h>

#pragma warning( push )
#pragma warning( disable: 4995 )	// warning C4995: 'vsprintf': name was marked as #pragma deprecated
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreRenderWindow.h>
#pragma warning( pop )

#define _USE_MATH_DEFINES
#undef M_PI
#include <math.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cModuleOgreApp;
using Loom::Speech::cModuleSpeech;
using Loom::TrackStar::cModuleTrackStar;

using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQOgre;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;
using Loom::Core::cSerializerXML;
using Loom::OgreApp::cOgreResponderCreateBox;
using Loom::OgreApp::IOgreEvent;

using namespace Loom::Avatar;

/************************************************************************/
cModuleAvatar::cModuleAvatar()
: IModule( _T( "Avatar" ) ), mConfig( NULL )
/************************************************************************/
{
	mAvatar1 = mAvatar2 = NULL;
	mDependencies.Add( cModuleOgreApp::GetName() );
	mDependencies.Add( cModuleSpeech::GetName() );
	mDependencies.Add( cModuleTrackStar::GetName() );
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

	// Load config
	std::wfstream *vStream = new std::wfstream();
	vStream->open( "resources/config/avatar.cfg", std::ios_base::in | std::ios_base::binary );
	if ( vStream->is_open() )
	{	// Open existing config
		cSerializerXML *vSerializer = new cSerializerXML( *vStream );
		mConfig = (cConfig*)vSerializer->Deserialize();
	}
	else
	{	// Create default config
		delete vStream;
		vStream = new std::wfstream();
		vStream->open( "resources/config/avatar.cfg", std::ios_base::out | std::ios_base::binary );
		cSerializerXML *vSerializer = new cSerializerXML( *vStream );
		mConfig = new cConfig();
		vSerializer->Serialize( mConfig );
	}
	vStream->close();
	delete vStream;

	CreateEnvironment();

 	mAvatar1 = new cOgreAvatar( "Avatar1", mConfig->Id1 );
	if ( stricmp( mConfig->Controller1, "Mouse" ) == 0 )
	{
		mAvatar1->SetController( new cControllerMouse );
	}
	else if ( stricmp( mConfig->Controller1, "Network" ) == 0 )
	{
		mAvatar1->SetController( new cControllerNetwork );
	}
	else if ( stricmp( mConfig->Controller1, "Track" ) == 0 )
	{
		mAvatar1->SetController( new cControllerTrack );
	}
	else if ( stricmp( mConfig->Controller1, "Test" ) == 0 )
	{
		mAvatar1->SetController( new cControllerTest );
	}
	else if ( stricmp( mConfig->Controller1, "Test2" ) == 0 )
	{
		mAvatar1->SetController( new cControllerTest2 );
	}
//	mAvatar1->SetController( new cControllerTrack );
//	mAvatar1->SetController( new cControllerTest );
//	mAvatar1->SetController( new cControllerMouse );
	mAvatar1->SetPosition( mConfig->Position );
	mAvatar1->SetRotation( Ogre::Quaternion( Ogre::Degree( mConfig->Orientation ), Ogre::Vector3( 0, 1, 0 ) ) );
	if ( !mConfig->SingleAvatar )
	{
		mAvatar2 = new cOgreAvatar( "Avatar2", mConfig->Id2 );
		mAvatar2->SetPosition( Ogre::Vector3( 0, 0, -120 ) );
		mAvatar2->SetRotation( Ogre::Quaternion( Ogre::Radian( 0 ), Ogre::Vector3( 0, 1, 0 ) ) );
//		mAvatar2->SetController( new cControllerInternal );
//		mAvatar2->SetEffectorPosition( EFFECTOR_LEFT_HAND, mAvatar2->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 5, 0, -15 ) );
//		mAvatar2->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mAvatar2->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( -14, -0.5f, -40 ) * 2 );
//		mAvatar2->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3( -14, -4.5f, 10 ) * 2 );
//		mAvatar2->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3( -14, -0.5f, 10 ) * 2 );
//		mAvatar2->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3( -14, -0.5, 120 - 57 ) );
//		mAvatar2->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3( -14, -0.5, 120 - 80 ) );
		
		Ogre::Quaternion vRot;
		vRot.FromAngleAxis( -Ogre::Radian( 0.4835988 ), Ogre::Vector3( 1, 0, 0 ) );
		mAvatar2->SetEffectorRotation( EFFECTOR_HEAD, vRot );

		if ( stricmp( mConfig->Controller2, "Mouse" ) == 0 )
		{
			mAvatar2->SetController( new cControllerMouse );
		}
		else if ( stricmp( mConfig->Controller2, "Network" ) == 0 )
		{
			mAvatar2->SetController( new cControllerNetwork );
		}
		else if ( stricmp( mConfig->Controller2, "Track" ) == 0 )
		{
			mAvatar2->SetController( new cControllerTrack );
		}
		else if ( stricmp( mConfig->Controller2, "Test" ) == 0 )
		{
			mAvatar2->SetController( new cControllerTest );
		}
		else if ( stricmp( mConfig->Controller2, "Test2" ) == 0 )
		{
			mAvatar2->SetController( new cControllerTest2 );
		}

//		mAvatar2->SetController( new cControllerMouse );
//		mAvatar2->SetController( new cControllerTest2 );
//		mAvatar2->GetController()->SetActive( false );

	}

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();
	mAvatar1->SetFPSCamera( vScene->getCamera( "SecondaryCamera" ) );

//	mAvatar2->OnAttach( Ogre::String( "Red cube" ), EFFECTOR_LEFT_HAND );
	mAvatar2->SetFPSCamera( vScene->getCamera( "MainCamera" ) );

//	vScene->getCamera( "SecondaryCamera" )->lookAt( Ogre::Vector3( 0, -10, -120 ) );
//	vScene->getCamera( "MainCamera" )->lookAt( Ogre::Vector3( 0, -10, 0 ) );

	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );

	if ( !mConfig->ViewportVisible1 )
	{
		Ogre::RenderWindow *vWindow = vQScene->GetWindow();
		vWindow->getViewport(0)->setAutoUpdated( false );
		vWindow->getViewport(1)->setDimensions( 0, 0, 1.0f, 1.0f );
	}
	if ( !mConfig->ViewportVisible1 )
	{
		Ogre::RenderWindow *vWindow = vQScene->GetWindow();
		vWindow->getViewport(1)->setAutoUpdated( false );
		vWindow->getViewport(0)->setDimensions( 0, 0, 1.0f, 1.0f );
	}

	mInitialized = true;
}

/************************************************************************/
DWORD cModuleAvatar::StartThread( LPVOID iParam )
/************************************************************************/
{

	cModuleAvatar* _this = (cModuleAvatar*) iParam;
	// First we need to do the calibration
//	cDispatcherHub::Get().Dispatch( _T("Speech::Say"), L"Let us do the calibration now..." );
//	Sleep(500);
//	cDispatcherHub::Get().Dispatch( _T("Speech::Say"), L"First move your arm down..." );
//	Sleep(500);

//	_this->mAvatar1->Calibrate();

	while ( true )
	{
		Sleep( 500 );

		if ( ( GetAsyncKeyState( 'C' ) & 0x8000 ) )
		{
			_this->mAvatar1->GetController()->SetActive( !(_this->mAvatar1->GetController()->IsActive()) );
			_this->mAvatar2->GetController()->SetActive( !(_this->mAvatar2->GetController()->IsActive()) );
		}
	}

	return 0;

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
//	vScene->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_MODULATIVE );
	
	vScene->getCamera( "MainCamera" )->setPosition( Ogre::Vector3( 0, 100, -200 ) );
	vScene->getCamera( "MainCamera" )->lookAt( Ogre::Vector3( 0, 0, 0 ) );
	vScene->getCamera( "SecondaryCamera" )->setPosition( Ogre::Vector3( 0, 100, -200 ) );
	vScene->getCamera( "SecondaryCamera" )->lookAt( Ogre::Vector3( 0, 0, 0 ) );

	// Create spotlight
	Ogre::Light* vLightSpot1;
	vScene->setAmbientLight( Ogre::ColourValue( 0.2f, 0.2f, 0.2f, 1.0f ) );
	vLightSpot1 = vScene->createLight("SpotLight1");
	vLightSpot1->setType(Ogre::Light::LT_SPOTLIGHT);
	//	vLightSpot1->setPosition(-900, 750, 375);
	vLightSpot1->setDirection(0.79, -0.38, 0.48);
	vLightSpot1->setPosition(-790, 380, -480);
	vLightSpot1->setDiffuseColour(0.5, 0.5, 0.5);
	vLightSpot1->setShadowFarDistance( 500.0f );
	vLightSpot1->setShadowFarClipDistance( 500.0f );
	vLightSpot1->setCastShadows( true );

	// Create spotlight
	Ogre::Light* vLightSpot2;
	vScene->setAmbientLight( Ogre::ColourValue( 0, 0, 0, 0 ) );
	vLightSpot2 = vScene->createLight("SpotLight2");
	vLightSpot2->setType(Ogre::Light::LT_SPOTLIGHT);
	//	vLightSpot2->setPosition(-900, 750, 375);
	vLightSpot2->setDirection(0.79, -0.38, -0.48);
	vLightSpot2->setPosition(-790, 380, 480);
	vLightSpot2->setDiffuseColour(0.5, 0.5, 0.5);
	vLightSpot2->setShadowFarDistance( 500.0f );
	vLightSpot2->setShadowFarClipDistance( 500.0f );
	vLightSpot2->setCastShadows( false );

	// Create table
//	vQScene->OnCreateBox( "Table", Ogre::Vector3( 0, -18, -25 ) * 2, Ogre::Vector3( 0.5f, 0.3f, 0.3f )*2 );
	vQScene->OnCreateBox( "Table", Ogre::Vector3( 0, -16	, -25 ) * 2, Ogre::Vector3( 0.5f, 0.3f, 0.3f )*2 );

	vQScene->OnCreateBox( "Blue cube 1", Ogre::Vector3(  14, 0, -13 ) * 2, Ogre::Vector3( 0.02f, 0.02f, 0.02f )*2 );
	vQScene->OnSetTextureFile( "Blue cube 1", "resources/textures/BlueCube1.jpg" );
	vQScene->OnCreateBox( "Blue cube 2", Ogre::Vector3( -14, 0, -37 ) * 2, Ogre::Vector3( 0.02f, 0.02f, 0.02f )*2 );
	vQScene->OnSetTextureFile( "Blue cube 2", "resources/textures/BlueCube2.jpg" );
	vQScene->OnCreateBox( "Red cube", Ogre::Vector3( -14, 0, -13 ) * 2, Ogre::Vector3( 0.02f, 0.02f, 0.02f )*2 );
	vQScene->OnSetTextureFile( "Red cube", "resources/textures/Red.jpg" );
	vQScene->OnCreateSphere( "Blue sphere", Ogre::Vector3( -5, -0.5f, -25 ) * 2, Ogre::Vector3( 0.02f, 0.02f, 0.02f )*2 );
	vQScene->OnSetTextureFile( "Blue sphere", "resources/textures/Blue.jpg" );
	vQScene->OnCreateSphere( "Red sphere", Ogre::Vector3(  5, -0.5f, -25 ) * 2, Ogre::Vector3( 0.02f, 0.02f, 0.02f )*2 );
	vQScene->OnSetTextureFile( "Red sphere", "resources/textures/Red.jpg" );

//	Ogre::RenderWindow *vWindow = vQScene->GetWindow();
//	vWindow->getViewport(0)->setDimensions( 0, 0, 0.5f, 1.0f );
//	vWindow->getViewport(0)->getCamera()->setAutoAspectRatio( true );

//	Ogre::Camera *vCamera = vScene->getCamera( "SecondaryCamera" );
//	Ogre::Viewport *vViewport = vWindow->addViewport( vCamera );
//	vWindow->getViewport(1)->setDimensions( 0.5f, 0, 0.5f, 1.0f );
//	vWindow->getViewport(1)->getCamera()->setAutoAspectRatio( true );


//	cAvatarResponderAttach::Get().Dispatch( &IAvatarEvent::OnAttach, Ogre::String( "Object_1" ), EFFECTOR_LEFT_HAND );
}

/************************************************************************/
cOgreAvatar * Loom::Avatar::cModuleAvatar::GetAvatar( const size_t iIndex ) const
/************************************************************************/
{
	if ( iIndex == 0 ) return mAvatar1;
	if ( iIndex == 1 ) return mAvatar2;

	return NULL;
}

/************************************************************************/
cOgreAvatar * Loom::Avatar::cModuleAvatar::GetAvatarById( const size_t iId ) const
/************************************************************************/
{
	if ( mAvatar1->GetId() == iId ) return mAvatar1;
	if ( mAvatar2->GetId() == iId ) return mAvatar2;

	return NULL;
}
