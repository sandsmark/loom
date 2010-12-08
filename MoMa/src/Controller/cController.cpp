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
	// Events
	Psyclone::cPsycloneSpeechOn::Get().AddListener( *this );
//	Speech::cSpeechResponderSay::Get().AddListener( *this );
	// Create stuff
	CreateCircles();
	CreateWaveform();
	CreateCreatures();

	// Test spew
	/*
	new cSpew( Ogre::Vector3( 25, 9, 100 ), Ogre::Vector3( 4, 0, 0 ) );
	cSpew *vSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -1, -4, 0 ) );
	vSpew->mTurbulenceStrength = 4;
	

	cSpew *vxSpew = new cSpew( Ogre::Vector3( 45, 9, 50 ), Ogre::Vector3( -1, 0, -1 ) );
	vxSpew->mTurbulenceStrength = 2;
	vxSpew->mDecay = 2;
	new cSpew( Ogre::Vector3( -45,   0,  50 ), Ogre::Vector3( -1, 0, -3 ) );
	cSpew *gSpew = new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( -5, -4, -10 ) );
	gSpew->mTurbulenceStrength = 2;
	new cSpew( Ogre::Vector3( -25, 9, 100 ), Ogre::Vector3( 4, -4, 7 ) );

	new cSpew( Ogre::Vector3( 45,   0,  50 ), Ogre::Vector3( -6, 8, 0 ) );

	new cSpew( Ogre::Vector3( 0, -20,   0 ), Ogre::Vector3( -3, 6, -5 ) );

	new cSpew( Ogre::Vector3( 0, -20,   0 ), Ogre::Vector3( 5, 0, 10 ) );

	new cSpew( Ogre::Vector3( 25,  20, 100 ), Ogre::Vector3( -7, 4, 10 ) );

	cSpew *hSpew = new cSpew( Ogre::Vector3( -45, 9, 50 ), Ogre::Vector3( 5, 0, 5 ) );
	hSpew->mTurbulenceStrength = 3;
	hSpew->mSpringStrength = 1;
	cSpew *iSpew = new cSpew( Ogre::Vector3( 25, 20, 100 ), Ogre::Vector3( 8, -4, 5 ) );
		iSpew->mTurbulenceStrength = 4;
		

	cSpew *bSpew = new cSpew( Ogre::Vector3( 25, 9, 100 ), Ogre::Vector3( 10, 0, -10 ) );	
	bSpew->mTurbulenceStrength = 25;
	bSpew->mDecay = 10;
	bSpew->mSpringStrength = 10;
	bSpew->mTurbulenceScale = 10;
	bSpew->mTurbulenceScroll = 10;
*/
	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
}

/************************************************************************/
DWORD cController::StartThread( LPVOID iParam )
/************************************************************************/
{
	srand ( time(NULL) );
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
void Loom::MoMa::cController::OnSpeechOn( const std::wstring &text )
/************************************************************************/
{
	
	int x = rand() % 20 - 10;
	int z = rand() % 20 - 10;
	int a = rand() % 20;
	int b = rand() % 20;
	int c = rand() % 20;
	WCHAR character = text[text.size()-1];
	Ogre::Vector3 location = Ogre::Vector3( 25, 9, 100 );
	if (character == '2')
		location = Ogre::Vector3( 0, -25, 0 );
	else if (character == '3')
		location = Ogre::Vector3( -45, -9, 50 );
	cSpew *bSpew = new cSpew( location, Ogre::Vector3( x, 0, z ) );	
	bSpew->mTurbulenceStrength = 5;
	bSpew->mDecay = 5;
	bSpew->mSpringStrength = a;
	bSpew->mTurbulenceScale = b;
	bSpew->mTurbulenceScroll = c;
	
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
