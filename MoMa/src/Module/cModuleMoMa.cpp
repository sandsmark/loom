#include <MoMa/Module/cModuleMoMa.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Ogre/OgreMath.h>
#include <Ogre/OgreColourValue.h>
#include <Ogre/OgreBillboardSet.h>
#include <MoMa/Entity/cPCreature.h>
#include <MoMa/Entity/cCreature.h>
#include <MoMa/Entity/cPCircle.h>
#include <MoMa/Entity/cCircle.h>
#include <MoMa/Entity/cWaveform.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <Core/Serializer/cSerializerXML.h>
#include <MoMa/Entity/cSpew.h>
#include <strsafe.h>
#include <sstream>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cModuleOgreApp;
using Loom::Core::cSerializerXML;

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
	cCreature *vCreature;
	vCreature = vPrototype->CreateInstance( Ogre::Vector3( -25,  20, 100 ) );
	vCreature = vPrototype->CreateInstance( Ogre::Vector3(  25,  20, 100 ) );
	vCreature = vPrototype->CreateInstance( Ogre::Vector3( -45,   0,  50 ) );
	vCreature = vPrototype->CreateInstance( Ogre::Vector3(  45,   0,  50 ) );
	vCreature = vPrototype->CreateInstance( Ogre::Vector3(   0, -20,   0 ) );
	vCreature;

//	cWaveform *vWaveform = new cWaveform();
//	vWaveform;

	cPCircle *vPCircle = new cPCircle();
	for ( int i=0; i<6; i++ )
	{
		cCircle *vCircle = vPCircle->CreateInstance();
		vCircle;
	}

	new cSpew( Ogre::Vector3( 25, 9, 100 ), Ogre::Vector3( 4, 0, 0 ) );
	cSpew *vSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	vSpew->mTurbulenceStrength = 4;

	/*
	cArray<cSpew::sNode> &vPoints = vSpew->mPoints;
	for ( int i=0; i<40; i++ )
	{
		cSpew::sNode vNode;
		vNode.Position = Ogre::Vector3( i, 0, 0 );
		vNode.PrevLength = 1;
		vNode.NextLength = 1;
		vNode.Length = i;
//		vNode.Velocity = Ogre::Vector3( Ogre::Math::RangeRandom( -1, 1 ), Ogre::Math::RangeRandom( -1, 1 ), Ogre::Math::RangeRandom( -1, 1 ) ) * 10;
//		vNode.Velocity = Ogre::Vector3( 0, 0, 0 );
		vNode.Velocity = Ogre::Vector3( Ogre::Math::RangeRandom( -1, 1 ), Ogre::Math::RangeRandom( -1, 1 ), 0 ) * 1;
		if ( i == 10 )
		{
//			vNode.Velocity *= 10;
			vNode.Velocity = Ogre::Vector3( 0, 1, 0 ) * 50;
		}
		vPoints.Add( vNode );
	}
	*/
	
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
