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
	userTurn = false;
	characterTurn = false;
	doBlink = false;
//	Speech::cSpeechResponderSay::Get().AddListener( *this );
	// Create stuff
	//CreateCircles();
	//CreateWaveform();
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
//	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3( -50,  20, 100 ) ) );
//	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3(  50,  20, 100 ) ) );
//	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3( -90,   0,  50 ) ) );
//	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3(  90,   0,  50 ) ) );
	mCreatures.Add( vPrototype->CreateInstance( Ogre::Vector3(   0, 10,   -100 ) ) );
}

/************************************************************************/
void Loom::MoMa::cController::OnSpeechOn( const std::wstring &text )
/************************************************************************/
{
	std::wstring type = L"";
	std::wstring character = L"";
	wchar_t delimiter = '.';
	if (text.compare(L"DiP.Other-gives-turn.Notify") == 0)
	{
		//30° to the side
		mCreatures[0]->Blink( 20);
		mCreatures[0]->SetEyeRotation( M_PI/6, 0 );
		doBlink = true;
	}
	if (text.compare(L"DiP.I-have-turn") == 0)
	{	
		
	}
	else // speechOn and SpeechOff
	{	
		std::wstring::size_type start = 0;
		std::wstring::size_type end   = text.find( delimiter, start );
		int i = 0;
		while( end != std::wstring::npos )
		{
			std::wstring token = text.substr( start, end - start );
			start = end + 1;
			end   = text.find( delimiter, start );
			if (i == 2)
				type = token;
			else if (i == 4)
				type = token;	
			i++;
		}
		character = text[text.size()-1];

		if (character.compare(L"4") == 0)  // character
		{
			if (type.compare(L"On") == 0)
			{		
				characterTurn = true;					
				prevEyeTarget = mCreatures[0]->GetEyeTarget();								
				mCreatures[0]->SetEyeRotation( 0, 0 );		
				mCreatures[0]->StartTransient( -2.0f );
			}
			else
			{
				characterTurn = false;
				mCreatures[0]->SetEyeRotation( 0, 0 );
			}
		} else  // person
		{
			if (type.compare(L"On") == 0)
			{		
				userTurn = true;		
			}
			else
			{
				userTurn = false;			
			}
		}
	}
		
	//mCreatures[4]->StartTransient( 4.0f );		
	//		mCreatures[0]->SetEyeRotation( 0, 0 );
	//		mCreatures[0]->StartTransient( 2.0f );		
	//mCreatures[0]->SetEyeDistortion(1.0f);
	//mCreatures[0]->SetEyeSize( 2.0f);
	//transient strength and size

	/*
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
*/	
		
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

	mCreatures[0]->SetHeadDistortionPosition( Ogre::Vector2( Ogre::Math::RangeRandom( -1, 1 ), Ogre::Math::RangeRandom( -1, 1 ) ) );
	mCreatures[0]->SetHeadDistortionStrength( Ogre::Math::RangeRandom( 0, 1 ) );

	int vIndex = rand() % mCreatures.GetSize();

	
	
	if ( ( rand() % 10 ) < 8 )
	{
		int vIndex = rand() % mCreatures.GetSize();
		if ( vIndex == 0 )
		{
			mCreatures[vIndex]->Blink( 8 );
		}
	}


	if (doBlink)
	{
		if (mCreatures[0]->GetEyeTarget().x == 0.0)
			mCreatures[vIndex]->SetEyeTarget(prevEyeTarget);
		mCreatures[vIndex]->Blink( 20 );
		if (rand() % 2 == 0)
			doBlink = false;
	}

	if (characterTurn)
	{
		float speed = (float)((rand() % 10) - 5);
		//if ( ( rand() % 10 ) < 12 )
		mCreatures[0]->StartTransient( speed );

		//random eye movement
		//10% straight, 90% wander		
		int look = rand() % 20;
		if (look < 1)  //80% straight
		{
			mCreatures[vIndex]->SetEyeRotation( 0, 0 );
		}
		else
		{
			float yaw = pow(2.0,rand() % 4 + 1);
			float pitch = pow(2.0,rand() % 5 + 2);
			switch (rand() % 4)
			{
			case 0:
				mCreatures[vIndex]->SetEyeRotation( -M_PI/yaw, 0 );			
				break;
			case 1:
				mCreatures[vIndex]->SetEyeRotation( -M_PI/yaw, -M_PI/pitch );			
				break;
			case 2:
				mCreatures[vIndex]->SetEyeRotation( M_PI/yaw, 0 );			
				break;
			case 3:
				mCreatures[vIndex]->SetEyeRotation( M_PI/yaw, -M_PI/pitch );			
				break;
			}

		}		
	

	}

	if (!characterTurn)
	{
		//random eye movement
		//80% straight, 20% 10-20° to the side	
		int look = rand() % 10;
		if (look < 7)  //80% straight
		{
			mCreatures[vIndex]->SetEyeRotation( 0, 0 );
		}
		else
		{
			//float yaw = pow(2.0,rand() % 5 + 1);
			float yaw = rand() % 7 + 1;
			//float pitch = pow(2.0,rand() % 6 + 3);
			switch (rand() % 4)
			{
			case 0:
					mCreatures[vIndex]->SetEyeRotation( -M_PI/yaw, 0 );			
					break;
			case 1:
			//		mCreatures[vIndex]->SetEyeRotation( -M_PI/yaw, -M_PI/pitch );			
					break;
			case 2:
					mCreatures[vIndex]->SetEyeRotation( M_PI/yaw, 0 );			
					break;
			case 3:
			//		mCreatures[vIndex]->SetEyeRotation( M_PI/yaw, -M_PI/pitch );			
					break;
			}


		
		}		
	
	}
}
