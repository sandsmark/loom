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

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cModuleOgreApp;
using Loom::Speech::cModuleSpeech;

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

	cOgreAvatar *vAvatar = new cOgreAvatar();
	vAvatar;

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
