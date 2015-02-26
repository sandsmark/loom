#include "StdAfx.h"
#include <Avatar/Controller/cControllerMouse.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Ogre/OgreVector3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "OgreApp/Module/cModuleOgreApp.h"
#include "../../../OgreApp/src/Qt/MainWindow/cQMainWindow.h"
#include "../../../OgreApp/src/Qt/Ogre/cQScene.h"
#include <Core/Module/cModuleManager.h>
#include <Ogre/OgreSceneNode.h>

using namespace Loom::Avatar;

using Loom::Core::cModuleManager;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQScene;

/************************************************************************/
cControllerMouse::cControllerMouse()
: mFrequency( 1 ), mTime( 0 ), mAvatar( NULL )
, mLeftHand( -28.3, 4, 80 ), mRightHand( 14, -0.5, 57 ), mHead( 0.4835988 )
, mGrab( false ), mPoint( false )
/************************************************************************/
{
	mLastUpdate = GetTickCount();
	mActive = true;
}

/************************************************************************/
cControllerMouse::~cControllerMouse()
/************************************************************************/
{
}

/************************************************************************/
void cControllerMouse::Init( cOgreAvatar *iAvatar )
/************************************************************************/
{
	ASSERTTXT( mAvatar == NULL, _T( "Avatar to control is already set!" ) );

	mAvatar = iAvatar;
	mAvatar->SetInteractive( true );

	Ogre::Quaternion vRot;
	vRot.FromAngleAxis( Ogre::Radian( 0 ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );

	mPointAlpha.SetSpeed( 8 );
	mPointAlpha.Init( 0 );
	mPointAlpha.Init( 0 );

	SetCursor( LoadCursor( NULL, NULL ) );
	ShowCursor( false );
}

/************************************************************************/
void cControllerMouse::Update()
/************************************************************************/
{
	if ( !mActive ) return;

	POINT vPoint;
	GetCursorPos( &vPoint );
	if ( mAvatar->GetPosition().z < -100 )
	{
		vPoint.x = 1440 - vPoint.x;
		vPoint.y =  800 - vPoint.y;
	}

	static Ogre::Vector3 vPos( 0, -0.5f, 0 );
	vPos.x = vPoint.x / 1440.0f * 80 - 40;
	if ( ( GetAsyncKeyState( VK_RBUTTON ) & 0x8000 ) )
	{
		vPos.y = ( 800 - vPoint.y ) / 800.0f * 30 - 10;
		if ( vPos.y < -14.0f ) vPos.y = -14.0f;
	}
	else
	{
		vPos.z = vPoint.y / 800.0f * 60 - 40;
	}
	if ( mAvatar->GetPosition().z < -100 )
	{
		vPos.z -= 80;
	}

	if ( ( GetAsyncKeyState( 'T' ) & 0x8000 ) )
	{
		vPos.y += 1.0f;
	}
	if ( ( GetAsyncKeyState( 'Y' ) & 0x8000 ) )
	{
		vPos.y -= 1.0f;
	}

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * vPos; 
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mRightHand );

	UpdateGrab();
	UpdatePoint();
}

/************************************************************************/
void Loom::Avatar::cControllerMouse::SetActive( bool iActive )
/************************************************************************/
{
	if ( mActive == iActive ) return;
	mActive = iActive;

	if ( mActive )
	{
		Ogre::Vector3 vPos = mAvatar->GetNode()->_getFullTransform() * mAvatar->mEffectorPositions[ EFFECTOR_RIGHT_HAND ];
		vPos.x += 40.0f;
		vPos.x /= 80.0f;
		vPos.x *= 1440.0f;
		if ( mAvatar->GetPosition().z < -100 )
		{
			vPos.z += 80;
		}
		vPos.z += 40.0f;
		vPos.z /= 60.0f;
		vPos.z *= 800.0f;
		if ( mAvatar->GetPosition().z < -100 )
		{
			vPos.x = 1440 - vPos.x;
			vPos.z =  800 - vPos.z;
		}
		SetCursorPos( (int)vPos.x, (int)vPos.z );
	}
}

/************************************************************************/
void Loom::Avatar::cControllerMouse::UpdateGrab( void )
/************************************************************************/
{
	static bool vPressed = false;
	if ( ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) )
	{
		vPressed = true;
	}
	else
	{
		if ( vPressed )
		{
			if ( !mGrab )
			{
				Ogre::Quaternion vRot;
				//				float vTarget = ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f;
				//	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
				vRot.FromAngleAxis( Ogre::Radian( ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
				mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
				mAvatar->OnGrab( mAvatar->GetId(), EFFECTOR_RIGHT_HAND );
			}
			else
			{
				Ogre::Quaternion vRot;
				//				float vTarget = ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f;
				//	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
				vRot.FromAngleAxis( Ogre::Radian( 0 ), Ogre::Vector3( 1, 0, 0 ) );
				mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
				mAvatar->OnRelease( mAvatar->GetId(), EFFECTOR_RIGHT_HAND );
			}
			mGrab = !mGrab;
		}
		vPressed = false;
	}
}

/************************************************************************/
void Loom::Avatar::cControllerMouse::UpdatePoint( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	float vPrevValue = mPointAlpha.GetValue();
	mPointAlpha.Update( vEllapsed );
	float vValue = mPointAlpha.GetValue();
	if ( fabsf( vPrevValue - vValue ) > 0.002f )
	{
		Ogre::Quaternion vRot;
		vRot.FromAngleAxis( Ogre::Radian( mPointAlpha.GetValue() ), Ogre::Vector3( 1, 0, 0 ) );
		mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_POINT, vRot );
	}

	static bool vPressed = false;
	if ( ( GetAsyncKeyState( 'P' ) & 0x8000 ) )
	{
		vPressed = true;
	}
	else
	{
		if ( vPressed )
		{
			if ( !mGrab )
			{
				Ogre::Quaternion vRot;
				vRot.FromAngleAxis( Ogre::Radian( ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
//				mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_POINT, vRot );
				mPointAlpha.SetTarget( ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f );
			}
			else
			{
				Ogre::Quaternion vRot;
				vRot.FromAngleAxis( Ogre::Radian( 0 ), Ogre::Vector3( 1, 0, 0 ) );
//				mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_POINT, vRot );
				mPointAlpha.SetTarget( 0 );
			}
			mGrab = !mGrab;
		}
		vPressed = false;
	}
}
