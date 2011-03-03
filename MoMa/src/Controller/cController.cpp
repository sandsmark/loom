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
	Psyclone::cDialogueStatus::Get().AddListener( *this );

	bUserTurn = false;
	bCharacterTurn = false;
	bDoBlink = false;
	bDoEyeGrow = false;
	lMoveEyeTimer = 0;
	lLookAwaytimer = 0;
	mSpeechState = SPEECH_OFF;

	x = 0;	
	y = 0;
	a1 = 0;
	b1 = 0;
	c1 = 0;
	d1 = 0;
	e1 = 0;
	f1 = 0;

	a2 = 0;
	b2 = 0;
	c2 = 0;
	d2 = 0;
	e2 = 0;
	f2 = 0;
	threshold = 0;
	// Create stuff
	CreateCircles();
	//CreateWaveform();
	CreateCreatures();

	mLastUpdate = GetTickCount();
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
	/*
	
	//	for ( int i=0; i<6; i++ )
		double y = 0.0;
		double x = 0.0;
		double xp = 0.0;
		double yp = 0.0;
		double a1 = (rand() % 20) / 10.0 - 1.0;
		double b1 = (rand() % 20) / 10.0 - 1.0;
		double c1 = (rand() % 20) / 10.0 - 1.0;
		double d1 = (rand() % 20) / 10.0 - 1.0;
		double e1 = (rand() % 20) / 10.0 - 1.0;
		double f1 = (rand() % 20) / 10.0 - 1.0;
	
		double a2 = (rand() % 20) / 10.0 - 1.0;
		double b2 = (rand() % 20) / 10.0 - 1.0;
		double c2 = (rand() % 20) / 10.0 - 1.0;
		double d2 = (rand() % 20) / 10.0 - 1.0;
		double e2 = (rand() % 20) / 10.0 - 1.0;
		double f2 = (rand() % 20) / 10.0 - 1.0;
	
		int threshold = rand() % 100;*/
	
	//for ( int i=0; i<500; i++ )
	//{
//		int prob = rand() % 99;
		double xp= rand() % 50;
		double yp= rand() % 50;
		cCircle *vCircle = vPCircle->CreateInstance();
		/*
		if (prob < threshold)
				{
					xp = a1*x+b1*y+c1;
					yp = d1*x+e1*y+f1;
				}
				else
				{
					xp = a2*x+b2*y+c2;
					yp = d2*x-e2*y+f2;
				}
				*/
		
		vCircle->SetPosition(Ogre::Vector2(xp,yp));
		vCircle->SetSize(Ogre::Vector2(6,6));
		mCircles.Add( vCircle );

		x = xp;
		y= yp;
//	}
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
	cCreature *vInstance = vPrototype->CreateInstance( Ogre::Vector3(   0, 10,   -10 ) );
	vInstance->SetScale( Ogre::Vector2( 0.5f, 0.5f ) );
	vInstance->SetPosition( Ogre::Vector3(   0, -10,   -100 ) );
	mCreatures.Add( vInstance );
}

double Loom::MoMa::cController::no1(double x)
{
	return 0.5*x*(1.0-x);
}

/************************************************************************/
void Loom::MoMa::cController::DialogueStatus( const std::wstring &text )
/************************************************************************/
{
	std::wstring type = L"";
	std::wstring character = L"";
	
	if (text.compare(L"Psyclone.Context:DiP.On.Intro") == 0)
	{
		y = 0.0;
		x = 0.0;
		
		a1 = (rand() % 20) / 10.0 - 1.0;
		b1 = (rand() % 20) / 10.0 - 1.0;
		c1 = (rand() % 20) / 10.0 - 1.0;
		d1 = (rand() % 20) / 10.0 - 1.0;
		e1 = (rand() % 20) / 10.0 - 1.0;
		f1 = (rand() % 20) / 10.0 - 1.0;

		a2 = (rand() % 20) / 10.0 - 1.0;
		b2 = (rand() % 20) / 10.0 - 1.0;
		c2 = (rand() % 20) / 10.0 - 1.0;
		d2 = (rand() % 20) / 10.0 - 1.0;
		e2 = (rand() % 20) / 10.0 - 1.0;
		f2 = (rand() % 20) / 10.0 - 1.0;

		threshold = rand() % 100;
	}
	else if (text.compare(L"DiP.Off") == 0)
	{
		mCircles.clear();
	}
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
		bDoBlink = true;
		mCreatures[0]->SetEyeType( 1 );
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
				bCharacterTurn = true;					
				vPrevEyeTarget = mCreatures[0]->GetEyeTarget();								
				mCreatures[0]->SetEyeRotation( 0, 0 );		
				mCreatures[0]->StartTransient( -2.0f );
				CreateCircles();
			}
			else
			{
				bCharacterTurn = false;
				mCreatures[0]->SetEyeRotation( 0, 0 );
			}
		} else  // person
		{
			if (type.compare(L".On") == 0)
			{		
				bUserTurn = true;		
			}
			else
			{
				bUserTurn = false;			
			}
		}
	}
		
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
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;


	// Circle test	
	//CreateCircles();
	//mCircles[0]->SetColour( Ogre::ColourValue( 141.0f / 255.0f, 248.0f / 255.0f, 170.0f / 255.0f, Ogre::Math::RangeRandom( 0.0f, 1.0f ) ) );
	//mCircles[0]->SetSize( Ogre::Vector2( Ogre::Math::RangeRandom( 8, 24 ), Ogre::Math::RangeRandom( 8, 24 ) ) );
	//mCircles[0]->SetRotation( Ogre::Radian( Ogre::Math::RangeRandom( 0, M_PI * 2.0f ) ) );
	if (bUserTurn)
	{
	/*
			cPCircle *vPCircle = new cPCircle();
				int prob = rand() % 99;
				double xp = 0.0;
				double yp = 0.0;
				cCircle *vCircle = vPCircle->CreateInstance();
				if (prob < threshold)
				{
					xp = a1*x+b1*y+c1;
					yp = d1*x+e1*y+f1;
				}
				else
				{
					xp = a2*x+b2*y+c2;
					yp = d2*x-e2*y+f2;
				}
		
				vCircle->SetPosition(Ogre::Vector2(xp,yp));
				vCircle->SetSize(Ogre::Vector2(6,6));
				mCircles.Add( vCircle );
		
				x = xp;
				y= yp;*/
		
/*
				foreach (cCircle c in mCircles)
				{
					c->SetRotation( Ogre::Radian( Ogre::Math::RangeRandom( 0, M_PI * 2.0f ) ) );
				}*/
		
	}
	else
	{
		cArray<cCircle*>::iterator it = mCircles.begin();
		for (it = mCircles.begin(); it < mCircles.end();it++)
		{
			Ogre::ColourValue cv = (*it)->GetColour();
			cv.a -= 0.05;
			(*it)->SetColour( cv );
		}
		
	}

	mNextUpdate -= vEllapsed;
	if ( mNextUpdate > 0 ) return;

	mNextUpdate = Ogre::Math::RangeRandom( 0.5f, 2.0f );

		/*
	mCreatures[0]->SetHeadDistortionPosition( Ogre::Vector2( Ogre::Math::RangeRandom( -1, 1 ), Ogre::Math::RangeRandom( -1, 1 ) ) );
	mCreatures[0]->SetHeadDistortionStrength( Ogre::Math::RangeRandom( 0, 1 ) );
	
	


	if (rand() % 20)
	{
		float d = rand()%10 / 10.0;
		mCreatures[0]->SetEyeDistortion(d);
	}

*/
	int vIndex = rand() % mCreatures.GetSize();
	if (bDoEyeGrow)
	{
		float grow = 1.0 - (rand() % 7 / 100.0);
		mCreatures[vIndex]->SetEyeSize(mCreatures[vIndex]->GetEyeSize()*grow);
	}
	mCreatures[0]->SetEyeDistortion(0.0f);
	
	// eye type test
	//mCreatures[0]->SetEyeType( ( ( rand() % 2 ) == 1 ) ? 1 : 0 );

	//random blink
	if ( ( rand() % 10 ) < 8 )
	{
		int vIndex = rand() % mCreatures.GetSize();
		if ( vIndex == 0 )
		{
			mCreatures[vIndex]->Blink( 8 );
		}
		mCreatures[vIndex]->SetEyeSize(1.0f);
		mCreatures[0]->SetEyeDistortion(0.0f);
		bDoEyeGrow = false;
		if (rand() % 2 == 1)
			bDoEyeGrow = true;
	}

	//take turn blink
	if (bDoBlink)
	{
		if (mCreatures[0]->GetEyeTarget().x == 0.0)
			mCreatures[vIndex]->SetEyeTarget(vPrevEyeTarget);
		mCreatures[vIndex]->Blink( 20 );
		if (rand() % 2 == 0)
			bDoBlink = false;		
	}

	if (bCharacterTurn)
	{
		float speed = (float)((rand() % 10) - 5);
		mCreatures[0]->StartTransient( speed );

		//random eye movement
		//10% straight, 90% wander		
		if (lMoveEyeTimer < GetTickCount())
		{		
			lMoveEyeTimer = GetTickCount() + 3000;
			if (rand() % 10 == 0)  //10% straight
			{
				mCreatures[vIndex]->SetEyeRotation( 0, 0 );
			}
			else
			{
				float yaw = pow(2.0,rand() % 5 + 2);
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
	}

	if (!bCharacterTurn)
	{
		//random eye movement
		//80% straight, 20% 10-20° to the side			
		if (lLookAwaytimer != 0 && lLookAwaytimer < GetTickCount())
		{
			mCreatures[vIndex]->SetEyeRotation( 0, 0 );
			lLookAwaytimer = 0;
		}
		else if (rand() % 10 < 7)  //80% straight
		{
			mCreatures[vIndex]->SetEyeRotation( 0, 0 );
		}
		else
		{
			lLookAwaytimer = GetTickCount() + 200;
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
