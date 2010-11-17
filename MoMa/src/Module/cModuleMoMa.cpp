#include <MoMa/Module/cModuleMoMa.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Ogre/OgreMath.h>
#include <Ogre/OgreColourValue.h>
#include <Ogre/OgreBillboardSet.h>
#include <MoMa/Entity/cPCreature.h>
#include <MoMa/Entity/cCreature.h>
#include <MoMa/Entity/cWaveform.h>
#include <OgreApp/Module/cModuleOgreApp.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cModuleOgreApp;

using namespace Loom::MoMa;

/************************************************************************/
cModuleMoMa::cModuleMoMa()
: IModule( _T( "MoMa" ) )
/************************************************************************/
{
	mDependencies.Add( cModuleOgreApp::GetName() );
}

/************************************************************************/
void cModuleMoMa::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleMoMa startup" ) );

//	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	// Test
	struct sTemp { Ogre::ColourValue Colour; };
	sTemp vParam;
	vParam.Colour.r = vParam.Colour.g = vParam.Colour.b = vParam.Colour.a = 0;
	cDispatcherHub::IParam vIParam( (void*)&vParam );
	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderSetBGColour"), vIParam );

	cPCreature *vPrototype = new cPCreature();
	cCreature *vCreature = vPrototype->CreateInstance();
	vCreature;

	cWaveform *vWaveform = new cWaveform();
	vWaveform;
	
	mInitialized = true;
}

/************************************************************************/
DWORD cModuleMoMa::StartThread( LPVOID iParam )
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
void cModuleMoMa::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );

	mInitialized = false;
}
