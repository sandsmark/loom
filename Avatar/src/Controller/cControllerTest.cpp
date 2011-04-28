#include "StdAfx.h"
#include <Avatar/Controller/cControllerTest.h>
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
cControllerTest::cControllerTest()
: mFrequency( 1 ), mTime( 0 ), mAvatar( NULL )
, mLeftHand( -28.3, 4, 80 ), mRightHand( 14, -0.5, 57 ), mHead( 0.4835988 )
, mStage( STAGE_INIT )
/************************************************************************/
{
	mLastUpdate = GetTickCount();
}

/************************************************************************/
cControllerTest::~cControllerTest()
/************************************************************************/
{
}

/************************************************************************/
void cControllerTest::Init( cOgreAvatar *iAvatar )
/************************************************************************/
{
	ASSERTTXT( mAvatar == NULL, _T( "Avatar to control is already set!" ) );

	mAvatar = iAvatar;

	int a = 0; a;
//	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND , Ogre::Vector3( -14, -0.5f, -13 ) );
//	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3(  14, -10.5f,  0 ) );

//	mRightHand = mAvatar->GetNode()->_getFullTransform() * mAvatar->GetEffectorPosition( EFFECTOR_RIGHT_HAND );
//	mLeftHand = mAvatar->GetNode()->_getFullTransform() * mAvatar->GetEffectorPosition( EFFECTOR_RIGHT_HAND );

	mLeftHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 14, -1, -40 ) * 2; 

	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND , mRightHand );
	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND , mLeftHand );

//	mAvatar->SolveLeft();

	mPrevPos = mRightHand;
	mPrevGrab = 0;

	Ogre::Quaternion vRot;
	vRot.FromAngleAxis( -Ogre::Radian( mHead ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_HEAD, vRot );

	vRot.FromAngleAxis( Ogre::Radian( 0 ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );

//	mAvatar->OnAttach( "Red cube", EFFECTOR_RIGHT_GRAB );
}

/************************************************************************/
void cControllerTest::Update()
/************************************************************************/
{
	static bool gPressed = false;
	if ( ( GetAsyncKeyState( VK_SPACE ) & 0x8000 ) )
	{
		gPressed = true;
	}
	else
	{
		if ( gPressed )
		{
			mPrevPos = mRightHand;
			NextStage();
		}
		gPressed = false;
	}

	switch ( mStage )
	{
	case STAGE_ONE:
		UpdateStageOne();
		break;
	case STAGE_TWO:
		UpdateStageTwo();
		break;
	case STAGE_THREE:
		UpdateStageThree();
		break;
	case STAGE_FOUR:
		UpdateStageFour();
		break;
	case STAGE_FIVE:
		UpdateStageFive();
		break;
	case STAGE_SIX:
		UpdateStageSix();
		break;
	case STAGE_SEVEN:
		UpdateStageSeven();
		break;
	case STAGE_EIGHT:
		UpdateStageEight();
		break;
	case STAGE_NINE:
		UpdateStageNine();
		break;
	case STAGE_TEN:
		UpdateStageTen();
		break;
	case STAGE_ELEVEN:
		UpdateStageEleven();
		break;
	case STAGE_TWELVE:
		UpdateStageTwelve();
		break;
	}

#if 0
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	if ( mAvatar == NULL ) return;

	mTime += vEllapsed;

	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND , mRightHand );
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND , mLeftHand );

	Ogre::Quaternion vRot;
	vRot.FromAngleAxis( -Ogre::Radian( mHead ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_HEAD, vRot );

	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime * 4 ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );

	if ( ( GetAsyncKeyState( 'Q' ) & 0x8000 ) ) mLeftHand.x += 1.1f;
	if ( ( GetAsyncKeyState( 'A' ) & 0x8000 ) ) mLeftHand.x -= 1.1f;
	if ( ( GetAsyncKeyState( 'W' ) & 0x8000 ) ) mLeftHand.y += 1.1f;
	if ( ( GetAsyncKeyState( 'S' ) & 0x8000 ) ) mLeftHand.y -= 1.1f;
	if ( ( GetAsyncKeyState( 'E' ) & 0x8000 ) ) mLeftHand.z += 1.1f;
	if ( ( GetAsyncKeyState( 'D' ) & 0x8000 ) ) mLeftHand.z -= 1.1f;
	if ( ( GetAsyncKeyState( 'R' ) & 0x8000 ) ) mHead += 0.02f;
	if ( ( GetAsyncKeyState( 'F' ) & 0x8000 ) ) mHead -= 0.02f;
#endif

	/*
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();

	Ogre::Matrix4 vTrans;
	vTrans = mAvatar->GetNode()->_getFullTransform();
	Ogre::Vector3 vDebugPos = vTrans * mRightHand;
	vQScene->OnSetPosition( "Red cube", vDebugPos );
	*/
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageOne( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();

	Ogre::Vector3 vPos;
	vQScene->OnGetPosition( "Red sphere", vPos );
//	vPos.y += 10.0f;
	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * vPos; 
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );

	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND, mLeftHand );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageTwo( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed;
	if ( vAlpha > 1 ) vAlpha = 1;

	mTime += vEllapsed;

	if ( mTime > 1 )
	{
//		mTime = 1;

		mAvatar->OnAttach( mAvatar->GetId(), "Red sphere", EFFECTOR_RIGHT_HAND );

		/*
		Ogre::Vector3 vPos = mAvatar->GetEffectorPosition( EFFECTOR_RIGHT_HAND );
		vPos.y += 30;
		mRightHand = vPos;
		*/
//		NextStage();
	}

	mPrevPos = mRightHand;

	Ogre::Quaternion vRot;
	float vTarget = ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f;
//	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
	vRot.FromAngleAxis( Ogre::Radian( mPrevGrab + ( vTarget - mPrevGrab ) * vAlpha ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageThree( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 0, 15, -10 ) * 2;
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );

	mPrevGrab = ( sinf( 1 ) * 0.5f - 0.5f ) * M_PI * 0.95f;

}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageFour( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( -5, -0.5f, -10 ) * 2;
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageFive( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 1;
	vAlpha -= vEllapsed;
	if ( vAlpha < 0 ) vAlpha = 0;

	mTime -= vEllapsed;

	if ( mTime < 0 )
	{
//		mTime = 1;

		mAvatar->OnDetach( mAvatar->GetId(), "Red sphere" );

		/*
		Ogre::Vector3 vPos = mAvatar->GetEffectorPosition( EFFECTOR_RIGHT_HAND );
		vPos.y += 30;
		mRightHand = vPos;
		*/
//		NextStage();
	}

	mPrevPos = mRightHand;

	Ogre::Quaternion vRot;
	float vTarget = ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f;
//	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
	vRot.FromAngleAxis( Ogre::Radian( mPrevGrab + ( vTarget - mPrevGrab ) * vAlpha ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageSix( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 14, -0.5f, -2 ) * 2.0f;
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageSeven( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();

	Ogre::Vector3 vPos;
	vQScene->OnGetPosition( "Red sphere", vPos );
//	vPos.y += 10.0f;
	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * vPos; 
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );

	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND, mLeftHand );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageEight( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed;
	if ( vAlpha > 1 ) vAlpha = 1;

	mTime += vEllapsed;

	if ( mTime > 1 )
	{
//		mTime = 1;

		mAvatar->OnAttach( mAvatar->GetId(), "Red sphere", EFFECTOR_RIGHT_HAND );

		/*
		Ogre::Vector3 vPos = mAvatar->GetEffectorPosition( EFFECTOR_RIGHT_HAND );
		vPos.y += 30;
		mRightHand = vPos;
		*/
//		NextStage();
	}

	mPrevPos = mRightHand;

	Ogre::Quaternion vRot;
	float vTarget = ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f;
//	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
	vRot.FromAngleAxis( Ogre::Radian( mPrevGrab + ( vTarget - mPrevGrab ) * vAlpha ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageNine( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 0, 15, -10 ) * 2;
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );

	mPrevGrab = ( sinf( 1 ) * 0.5f - 0.5f ) * M_PI * 0.95f;

}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageTen( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 12, -0.5f, -5 ) * 2;
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageEleven( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 1;
	vAlpha -= vEllapsed;
	if ( vAlpha < 0 ) vAlpha = 0;

	mTime -= vEllapsed;

	if ( mTime < 0 )
	{
//		mTime = 1;

		mAvatar->OnDetach( mAvatar->GetId(), "Red sphere" );

		/*
		Ogre::Vector3 vPos = mAvatar->GetEffectorPosition( EFFECTOR_RIGHT_HAND );
		vPos.y += 30;
		mRightHand = vPos;
		*/
//		NextStage();
	}

	mPrevPos = mRightHand;

	Ogre::Quaternion vRot;
	float vTarget = ( sinf( 0 ) * 0.5f - 0.5f ) * M_PI * 0.95f;
//	vRot.FromAngleAxis( Ogre::Radian( ( sinf( mTime ) * 0.5f - 0.5f ) * M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
	vRot.FromAngleAxis( Ogre::Radian( mPrevGrab + ( vTarget - mPrevGrab ) * vAlpha ), Ogre::Vector3( 1, 0, 0 ) );
	mAvatar->SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );
}

/************************************************************************/
void Loom::Avatar::cControllerTest::UpdateStageTwelve( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;
	static float vAlpha = 0;
	vAlpha += vEllapsed * 1.0f;
	if ( vAlpha > 1 ) vAlpha = 1;

	mRightHand = mAvatar->GetNode()->_getFullTransform().inverse() * Ogre::Vector3( 14, -0.5f, -2 ) * 2.0f;
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, mPrevPos + ( mRightHand - mPrevPos ) * vAlpha );
}

