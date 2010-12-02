#include <MoMa/Controller/cController.h>
#include <Ogre/OgreMath.h>
#include <Ogre/OgreColourValue.h>
#include <Ogre/OgreBillboardSet.h>
#include <MoMa/Entity/cPCreature.h>
#include <MoMa/Entity/cCreature.h>
#include <MoMa/Entity/cPCircle.h>
#include <MoMa/Entity/cCircle.h>
#include <MoMa/Entity/cWaveform.h>
#include <Core/Serializer/cSerializerXML.h>
#include <MoMa/Entity/cSpew.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Loom::MoMa;

using Loom::Core::cDispatcherHub;
using Loom::Core::cSerializerXML;

/************************************************************************/
cController::cController()
/************************************************************************/
{
}

cController::~cController()
{
	TerminateThread( mThread, 0 );
}

/************************************************************************/
void cController::Run( void )
/************************************************************************/
{
	Init();
}

void cController::Init( void )
{
	// Set a black background
	struct sTemp { Ogre::ColourValue Colour; };
	sTemp vParam;
	vParam.Colour.r = vParam.Colour.g = vParam.Colour.b = vParam.Colour.a = 0;
	cDispatcherHub::IParam vIParam( (void*)&vParam );
	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderSetBGColour"), vIParam );

	// Create stuff
	CreateCircles();
	CreateWaveform();
	CreateCreatures();

	// Test spew
	/*
	new cSpew( Ogre::Vector3( 25, 9, 100 ), Ogre::Vector3( 4, 0, 0 ) );
	cSpew *vSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	vSpew->mTurbulenceStrength = 4;
	*/

	cSpew *vxSpew = new cSpew( Ogre::Vector3( 45, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	vxSpew->mTurbulenceStrength = 2;

	new cSpew( Ogre::Vector3( -45,   0,  50 ), Ogre::Vector3( -1, -4, 0 ) );
	cSpew *gSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	gSpew->mTurbulenceStrength = 2;
	new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );

	new cSpew( Ogre::Vector3( 45,   0,  50 ), Ogre::Vector3( -1, -4, 0 ) );

	new cSpew( Ogre::Vector3( 0, -20,   0 ), Ogre::Vector3( -1, -4, 0 ) );

	new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );

	new cSpew( Ogre::Vector3( 25,  20, 100 ), Ogre::Vector3( -1, -4, 0 ) );

	cSpew *hSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	hSpew->mTurbulenceStrength = 3;
	hSpew->mSpringStrength = 10;
	cSpew *iSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	iSpew->mTurbulenceStrength = 4;
	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
}

/************************************************************************/
DWORD cController::StartThread( LPVOID iParam )
/************************************************************************/
{
	cController *vController = (cController*)iParam;
	while ( true )
	{
		Sleep( 2 );
		vController->Update();
	}
}


/************************************************************************/
void Loom::MoMa::cController::CreateWaveform()
/************************************************************************/
{
	new cWaveform();
}

/************************************************************************/
void Loom::MoMa::cController::CreateCircles()
/************************************************************************/
{
	cPCircle *vPCircle = new cPCircle();
	for ( int i=0; i<6; i++ )
	{
		cCircle *vCircle = vPCircle->CreateInstance();
		vCircle;
	}
}

/************************************************************************/
void Loom::MoMa::cController::CreateCreatures()
/************************************************************************/
{
	cPCreature *vPrototype = new cPCreature();
	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3( -25,  20, 100 ) ) );
	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3(  25,  20, 100 ) ) );
	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3( -45,   0,  50 ) ) );
	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3(  45,   0,  50 ) ) );
	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3(   0, -20,   0 ) ) );
}

/************************************************************************/
void Loom::MoMa::cController::OnHeard( const std::wstring &text )
/************************************************************************/
{
	int a = 0;
	a;
}

/************************************************************************/
void Loom::MoMa::cController::Destroy( void )
/************************************************************************/
{
	for ( size_t i=0; i<mCreatures.GetSize(); i++ )
	{
		SAFE_DELETE( mCreatures[i] );
	}
}

/************************************************************************/
void Loom::MoMa::cController::Update( void )
/************************************************************************/
{
	Sleep( ( rand() % 1500 ) + 500 );

	int vIndex = rand() % mCreatures.GetSize();
	switch ( rand() % 5 )
	{
	case 0:
		mCreatures[vIndex]->RotateEye( 0, 0 );
		break;
	case 1:
		mCreatures[vIndex]->RotateEye( -M_PI_2, 0 );
		break;
	case 2:
		mCreatures[vIndex]->RotateEye(  M_PI_2, 0 );
		break;
	case 3:
		mCreatures[vIndex]->RotateEye( 0, -M_PI_2 );
		break;
	case 4:
		mCreatures[vIndex]->RotateEye( 0,  M_PI_2 );
		break;
	}
}
