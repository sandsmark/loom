#include "StdAfx.h"
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Avatar/IKChain/IKChain.h>
#include <Avatar/IKChain/IKJoint.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreSkeletonInstance.h>
#include <Ogre/OgreEntity.h>
#include <Ogre/OgreLight.h>
#include <Ogre/OgreQuaternion.h>
#include <Ogre/OgreAxisAlignedBox.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Avatar/IKChain/IKChain.h>
#include <Avatar/Event/cAvatarResponder.h>
#include <Avatar/Controller/IController.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>
#include <TrackStar/TrackStar.h>
#include <TrackStar/Module/cModuleTrackStar.h>
#include <TrackStar/Module/TrackStarInterface.h>
#include <strsafe.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Ogre/OgreCamera.h>
#include <Avatar/Event/Responders/cAvatarResponderGrab.h>
#include <Avatar/Event/Responders/cAvatarResponderRelease.h>

using namespace Loom::Avatar;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQOgre;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;
using Loom::Avatar::cAvatarResponderSetEffectorPosition;
using Loom::OgreApp::cOgreResponderOnRender;
using Loom::TrackStar::cModuleTrackStar;
using Loom::TrackStar::TrackStarInterface;
using Loom::Core::cLogger;
using Loom::Avatar::cAvatarResponderGrab;
using Loom::Avatar::cAvatarResponderRelease;

/************************************************************************/
cOgreAvatar::cOgreAvatar( const Ogre::String &iName, const size_t iId )
: mController( NULL ), mFPSCamera( NULL ), mId( iId ), mInteractive( false )
/************************************************************************/
{
	mCalibration = Ogre::Matrix4::IDENTITY;
	/*
	*(Ogre::Vector3*)mCalibration[0] = Ogre::Vector3( -1,  0,  0 );
	*(Ogre::Vector3*)mCalibration[1] = Ogre::Vector3(  0, -1,  0 );
	*(Ogre::Vector3*)mCalibration[2] = Ogre::Vector3(  0,  0, -1 );
	*(Ogre::Vector3*)mCalibration[3] = Ogre::Vector3(  0,  0,  0 );
	*/

	CreateEntity( iName );
	CreateIkChain();

	// Subscribe for Avatar messages
	cAvatarResponderSetEffectorPosition::Get().AddListener( *this );
	cAvatarResponderSetEffectorRotation::Get().AddListener( *this );
	cAvatarResponderGrab::Get().AddListener( *this );
	cAvatarResponderRelease::Get().AddListener( *this );
	Speech::cSpeechResponderHeard::Get().AddListener( *this );

	// Subscribe for Ogre rendering message
	cOgreResponderOnRender::Get().AddListener( *this );

	memset( &mGrab, 0, sizeof( mGrab ) * sizeof( bool ) );

	CreateLines();

//	SetEffectorPosition( EFFECTOR_LEFT_HAND , Ogre::Vector3( -200, 200, 0 ) );
//	SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3(  200, 200, 0 ) );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::CreateEntity( const Ogre::String &iName )
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	// Create humanoid
	mEntity = vScene->createEntity( iName, "models/SuperHumanoid.mesh" );
//	mNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 200, 340, -200 ) );
	Ogre::Quaternion vRotation;
	vRotation.FromAngleAxis( Ogre::Radian( Ogre::Math::PI ), Ogre::Vector3( 0, 1, 0 ) );
	mNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 0, 0 ), vRotation );
//	mNode->setScale( 0.5f, 0.5f, 0.5f );
	mEntity->setMaterialName( "Avatar/Humanoid" );
	mEntity->setCastShadows( true );
	mEntity->setDisplaySkeleton( false );
//	mEntity->setDisplaySkeleton( true );
	mEntity->setCastShadows( false );

	mNode->attachObject( mEntity );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::CreateIkChain()
/************************************************************************/
{
	// Left IK chain
	{
		mIKChainLeft = new IKChain(mNode);
		mSkeleton = mEntity->getSkeleton();
		Ogre::Bone* vBone = mSkeleton->getBone("TailBone");
		mIKChainLeft->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("Spine_1");
		mIKChainLeft->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("Spine_2");
		mIKChainLeft->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("ShoulderLeft");
		mIKChainLeft->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("ArmUpperLeft");
		mIKChainLeft->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("ArmLowerLeft");
		mIKChainLeft->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("HandLeft");
		mIKChainLeft->PushOgreBone(vBone);

		// Lower Spine
		IKJoint* vBase = mIKChainLeft->GetJointAtDepth(1);
		vBase->SetJointAngleSymmetric(Ogre::Math::PI/16);
		vBase->SetWeight(0.1);
		// Upper Spine
		vBase = mIKChainLeft->GetJointAtDepth(2); 
		vBase->SetJointAngleSymmetric(Ogre::Math::PI/16);
		vBase->SetWeight(0.1);
		// Shoulder
		vBase = mIKChainLeft->GetJointAtDepth(3); //rs
		vBase->SetJointAngleSymmetric(0);
		vBase->SetWeight(0.1);
		// Upper arm
		vBase = mIKChainLeft->GetJointAtDepth(4); //ua
		vBase->SetMinJointAngleX(0);
		vBase->SetMaxJointAngleX(0);
		/*
		vBase->SetMinJointAngleY(M_PI/2);
		vBase->SetMaxJointAngleY(M_PI/2);
		vBase->SetMinJointAngleZ(M_PI/2);
		vBase->SetMaxJointAngleZ(M_PI/2);
		*/
		// Lower Arm
		vBase = mIKChainLeft->GetJointAtDepth(5); //la
		vBase->SetMinJointAngleX(0);
		vBase->SetMaxJointAngleX(0);
		/*
		vBase->SetMinJointAngleY(M_PI/2);
		vBase->SetMaxJointAngleY(M_PI/2);
		vBase->SetMinJointAngleZ(M_PI/2);
		vBase->SetMaxJointAngleZ(M_PI/2);
		*/
	}

	// Right IK chain
	{
		mIKChainRight = new IKChain(mNode);
		mSkeleton = mEntity->getSkeleton();
		Ogre::Bone* vBone = mSkeleton->getBone("TailBone");
		mIKChainRight->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("Spine_1");
		mIKChainRight->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("Spine_2");
		mIKChainRight->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("ShoulderRight");
		mIKChainRight->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("UpperArmRight");
		mIKChainRight->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("LowerArmRight");
		mIKChainRight->PushOgreBone(vBone);
		vBone = mSkeleton->getBone("HandRight");
		mIKChainRight->PushOgreBone(vBone);

		// Lower Spine
		IKJoint* vBase = mIKChainRight->GetJointAtDepth(1);
		vBase->SetJointAngleSymmetric(Ogre::Math::PI/16);
		vBase->SetMaxJointAngleX(Ogre::Math::PI/2);
		vBase->SetMinJointAngleX(-Ogre::Math::PI/2);
//		vBase->SetMaxJointAngleY(Ogre::Math::PI/2);
//		vBase->SetMinJointAngleY(-Ogre::Math::PI/2);
//		vBase->SetMaxJointAngleZ(Ogre::Math::PI/2);
//		vBase->SetMinJointAngleZ(-Ogre::Math::PI/2);
//		vBase->SetWeight(0.1);
#if 0
		// Upper Spine
		vBase = mIKChainRight->GetJointAtDepth(2); 
		vBase->SetJointAngleSymmetric(Ogre::Math::PI/16);
		vBase->SetWeight(0.1);
		// Shoulder
		vBase = mIKChainRight->GetJointAtDepth(3); //rs
		vBase->SetJointAngleSymmetric(0);
		vBase->SetWeight(0.1);
		// Upper arm
		vBase = mIKChainRight->GetJointAtDepth(4); //ua
		vBase->SetMinJointAngleX(0);
		vBase->SetMaxJointAngleX(0);
		// Lower Arm
		vBase = mIKChainRight->GetJointAtDepth(5); //la
		vBase->SetMinJointAngleX(0);
		vBase->SetMaxJointAngleX(0);
#endif
	}

	Ogre::Bone *vBone = mSkeleton->getBone("ArmUpperLeft");
	vBone->setManuallyControlled( true );
	Ogre::Quaternion vRot;
	vRot.FromAngleAxis( Ogre::Radian( -1.3f ), Ogre::Vector3( 1, 0, 0 ) );
	vBone->setOrientation( vRot );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPos )
/************************************************************************/
{
	Ogre::Vector4 vPos( iPos.x, iPos.y, iPos.z, 1 );
	vPos = vPos * mCalibration;
//	if ( ( mEffectorPositions[ iEffector ] - Ogre::Vector3( vPos.x, vPos.y, vPos.z ) ).length() < 0.01f ) return;
	mEffectorPositions[ iEffector ] = Ogre::Vector3( vPos.x, vPos.y, vPos.z );

	/*
	for ( int i=0; i<10; i++ )
	{
		mIKChainLeft->Solve( mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
		mIKChainRight->Solve( mEffectorPositions[ EFFECTOR_RIGHT_HAND ] );
	}
	*/

	switch ( iEffector )
	{
	case EFFECTOR_LEFT_HAND:
//		mIKChainRight->Solve( mEffectorPositions[ EFFECTOR_RIGHT_HAND ] );
//		mIKChainLeft->Solve( mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
		break;
	case EFFECTOR_RIGHT_HAND:
//		mIKChainLeft->Solve( mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
		Ogre::Quaternion vRot = mSkeleton->getBone( "HandRight" )->_getDerivedOrientation();
		Ogre::Vector3 vX, vY, vZ;
		vRot.ToAxes( vX, vY, vZ );
		static float vXOffset = 10.6;	//10;
		static float vYOffset = 11.2;	//5;
		static float vZOffset = 2.2;		//3
		Ogre::Vector3 vPos = mEffectorPositions[ EFFECTOR_RIGHT_HAND ] - vY * vXOffset;
		vPos += vZ * vYOffset;
		vPos -= vX * vZOffset;

		if ( vPos.y < 0.0f ) vPos.y = 0;

		if ( ( GetAsyncKeyState( 'U' ) & 0x8000 ) ) vXOffset += 0.1f;
		if ( ( GetAsyncKeyState( 'J' ) & 0x8000 ) ) vXOffset -= 0.1f;
		if ( ( GetAsyncKeyState( 'I' ) & 0x8000 ) ) vYOffset += 0.1f;
		if ( ( GetAsyncKeyState( 'K' ) & 0x8000 ) ) vYOffset -= 0.1f;
		if ( ( GetAsyncKeyState( 'O' ) & 0x8000 ) ) vZOffset += 0.1f;
		if ( ( GetAsyncKeyState( 'L' ) & 0x8000 ) ) vZOffset -= 0.1f;

		mIKChainRight->Solve( vPos );
		/*
		if ( mController )
		{
			cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
			cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
			cQScene *vQScene = vQMainWindow->GetScene();

			Ogre::Matrix4 vTrans;
			vTrans = GetNode()->_getFullTransform();
			Ogre::Vector3 vDebugPos = vTrans * vPos;
			vQScene->OnSetPosition( "Red sphere", vDebugPos );
		}
		*/
		break;
	}

}

/************************************************************************/
const Ogre::Vector3 Loom::Avatar::cOgreAvatar::GetEffectorPosition( const eEffector iEffector )
/************************************************************************/
{
	switch ( iEffector )
	{
	case EFFECTOR_LEFT_HAND:
		return mIKChainLeft->GetEffectorPosition();
		break;
	case EFFECTOR_RIGHT_HAND:
		return mIKChainRight->GetEffectorPosition();
		break;
	case EFFECTOR_HEAD:
		{
			Ogre::Bone* vHead = mSkeleton->getBone( "Head" );
			return vHead->_getDerivedPosition();
		}
	case EFFECTOR_LEFT_GRAB:
		{
			Ogre::Bone* vBone = mSkeleton->getBone("PinkyLeft");
			return vBone->_getDerivedPosition();
		}
	}

	return Ogre::Vector3( 0, 0, 0 );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnSetEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	if ( mController ) return;
	if ( iId != mId ) return;

	SetEffectorPosition( iEffector, mNode->_getFullTransform().inverse() * iPosition );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::Calibrate( void )
/************************************************************************/
{
	mCalibrationState = CALIBRATION_STATE1;
	cDispatcherHub::Get().Dispatch( _T("Speech::Say"), L"Calibration 1" );

	mCalibration = Ogre::Matrix4::IDENTITY;

	Ogre::Vector3 vLeftPos( -0.5f, -0.5f, 0.0f );
	Ogre::Vector3 vRightPos( 0.5f, 0.5f, 0.0f );
	Ogre::Vector3 vTopPos( 0.0f, 0.0f, 1.0f );
	Ogre::Vector3 vMidPos = ( vLeftPos + vRightPos ) * 0.5f;

	Ogre::Vector3 vRight = ( vRightPos - vLeftPos );
	vRight *= 1.0f / vRight.length();
	Ogre::Vector3 vUp = ( vTopPos - vMidPos );
	vUp.normalise();
	vUp *= vRight.length();
	Ogre::Vector3 vFront = vRight.normalisedCopy().crossProduct( vUp.normalisedCopy() ).normalisedCopy();
	vFront *= vRight.length();

	*((Ogre::Vector3*)mCalibration[0]) = vRight;
	*((Ogre::Vector3*)mCalibration[1]) = vUp;
	*((Ogre::Vector3*)mCalibration[2]) = vFront;

//	mCalibration = mCalibration.inverse();

	Ogre::Vector3 vTest = vRightPos - vLeftPos;
	vTest = mCalibration.transformAffine( vTest );

	int a = 0; a;
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnHeard( const std::wstring &text )
/************************************************************************/
{
	const int vHeadSensorId = 0;
	const int vLeftArmSensorId  = 1;
	const int vRightArmSensorId = 2;

//	if ( ( text != L"done" ) && ( text != L"ok" ) ) return;

	if ( text.length() < 10 ) return;

	cModuleTrackStar *vTrackStar = (cModuleTrackStar*)cModuleManager::Get().GetModule( cModuleTrackStar::GetName() );
	if (!vTrackStar) return;
	TrackStarInterface *vInterface = vTrackStar->GetInterface();
	if (!vInterface) return;

	switch ( mCalibrationState )
	{
	case CALIBRATION_STATE1:
		{	// X-axis, arms sideways
			// Get calibration data

			Ogre::Vector3 vLeft;
			Ogre::Vector3 vRight;

			double vX, vY, vZ, vA, vE, vR;

			while ( !vInterface->getNextEntryConst( vLeftArmSensorId, vX, vY, vZ, vA, vE, vR, 2000 ) );
			vLeft = Ogre::Vector3( vX, vY, vZ );
			while ( !vInterface->getNextEntryConst( vRightArmSensorId, vX, vY, vZ, vA, vE, vR, 2000 ) );
			vRight = Ogre::Vector3( vX, vY, vZ );

			Ogre::Vector3 vOrigLeft  = mSkeleton->getBone( "HandLeft" )->_getDerivedPosition();
			Ogre::Vector3 vOrigRight = mSkeleton->getBone( "HandRight" )->_getDerivedPosition();

			mTestLeft  = vLeft;
			mTestRight = vRight;
			mMidPoint = ( vLeft + vRight ) * 0.5f;

			Ogre::Vector3 vAxis = vRight - vLeft;
			float vOrigLength = ( vOrigRight - vOrigLeft ).length();
			mScale = vOrigLength / vAxis.length();
//			mScale = vOrigLength; // / vAxis.length();
//			vAxis *= mScale;
			vAxis.normalise();
			*((Ogre::Vector3*)mCalibration[0]) = vAxis;

			// Next
			cDispatcherHub::Get().Dispatch( _T("Speech::Say"), L"Calibration 2" );
			mCalibrationState = CALIBRATION_STATE2;
		}
		break;
	case CALIBRATION_STATE2:
		{	// Y-axis, arms up
			// Get calibration data

			Ogre::Vector3 vLeft;
			Ogre::Vector3 vRight;
			Ogre::Vector3 vHead;
			double vX, vY, vZ, vA, vE, vR;

			while ( !vInterface->getNextEntryConst( vLeftArmSensorId, vX, vY, vZ, vA, vE, vR, 2000 ) );
			vLeft = Ogre::Vector3( vX, vY, vZ );
			while ( !vInterface->getNextEntryConst( vRightArmSensorId, vX, vY, vZ, vA, vE, vR, 2000 ) );
			vRight = Ogre::Vector3( vX, vY, vZ );
			while ( !vInterface->getNextEntryConst( vHeadSensorId, vX, vY, vZ, vA, vE, vR, 2000 ) );
			vHead = Ogre::Vector3( vX, vY, vZ );

			Ogre::Vector3 vOrigLeft  = mSkeleton->getBone( "HandLeft" )->_getDerivedPosition();
			Ogre::Vector3 vOrigRight = mSkeleton->getBone( "HandRight" )->_getDerivedPosition();
			Ogre::Vector3 vOrigHead  = mSkeleton->getBone( "Head" )->_getDerivedPosition();

			Ogre::AxisAlignedBox vBounds = mEntity->getBoundingBox();
			Ogre::Vector3 vMid = ( vLeft + vRight ) * 0.5f;
			Ogre::Vector3 vAxis = vHead - vMid;
			vAxis.normalise();
			vAxis *= ((Ogre::Vector3*)mCalibration[0])->length();
			*((Ogre::Vector3*)mCalibration[1]) = vAxis;

			Ogre::Vector3 vRightAxis = *((Ogre::Vector3*)mCalibration[0]);
			Ogre::Vector3 vUpAxis = *((Ogre::Vector3*)mCalibration[1]);
			vRightAxis.normalise();
			vUpAxis.normalise();
			Ogre::Vector3 vFrontAxis = vRightAxis.crossProduct( vUpAxis );
			vFrontAxis.normalise();
			vFrontAxis *= ((Ogre::Vector3*)mCalibration[0])->length();
			*((Ogre::Vector3*)mCalibration[2]) = vFrontAxis;

			// Calibration space to normal space
//			mCalibration = mCalibration.inverse();
			
			mCalibration = mCalibration.transpose();
			mCalibration = mCalibration.inverse();

			mCalibration = mCalibration * Ogre::Matrix4::getScale( mScale, mScale, mScale );
			mCalibration[3][3] = 1;

			Ogre::Vector3 vNewMidPoint = mCalibration.transformAffine( mMidPoint );
			Ogre::Vector3 vOffset = ( ( vOrigLeft + vOrigRight ) * 0.5f ) - vNewMidPoint;
			mCalibration = Ogre::Matrix4::getTrans( vOffset ) * mCalibration;

			Ogre::Vector3 vTestLeft = mCalibration.transformAffine( mTestLeft );
			Ogre::Vector3 vTestRight = mCalibration.transformAffine( mTestRight );
			Ogre::Vector3 vTestDir = mCalibration.transformAffine( mTestRight - mTestLeft );

			// Next ( don't need calibration state 3! )
			cDispatcherHub::Get().Dispatch( _T("Speech::Say"), L"Calibration done" );
			mCalibrationState = CALIBRATION_DONE;
		}
		break;
	}
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation )
/************************************************************************/
{
	switch ( iEffector )
	{
	case EFFECTOR_HEAD:
		{
			Ogre::Matrix3 vMatrix;
			mCalibration.extract3x3Matrix( vMatrix );
			Ogre::Quaternion vTransform( vMatrix );
			Ogre::Quaternion vRotation = iRotation * vTransform;

			/*
			Ogre::Bone* vNeck = mSkeleton->getBone( "Neck" );
			vNeck->setManuallyControlled( true );
			vNeck->setOrientation( iRotation );
			*/
			Ogre::Bone* vHead = mSkeleton->getBone( "Head" );
			vHead->setManuallyControlled( true );
			vHead->setOrientation( vRotation );
		}
		break;
	case EFFECTOR_LEFT_GRAB:
		{
			Ogre::Bone* vHead;
			vHead = mSkeleton->getBone( "IndexLeft" );
			vHead->setManuallyControlled( true );
			vHead->setOrientation( iRotation );
			vHead = mSkeleton->getBone( "PinkyLeft" );
			vHead->setManuallyControlled( true );
			vHead->setOrientation( iRotation );
			vHead = mSkeleton->getBone( "ThumbLeft" );
			vHead->setManuallyControlled( true );
			Ogre::Quaternion vTemp;
			vTemp.FromAngleAxis( Ogre::Radian( M_PI / 2 ), Ogre::Vector3( 0, 0, 1 ) );
			Ogre::Quaternion vThumb = iRotation;
			vThumb.x *= 0.25f;
			vHead->setOrientation( vTemp * vThumb );
		}
		break;
	case EFFECTOR_RIGHT_GRAB:
		{
			Ogre::Bone* vHead;
			vHead = mSkeleton->getBone( "IndexRight" );
			vHead->setManuallyControlled( true );
			vHead->setOrientation( iRotation );
			vHead = mSkeleton->getBone( "PinkyRight" );
			vHead->setManuallyControlled( true );
			vHead->setOrientation( iRotation );
			vHead = mSkeleton->getBone( "ThumbRight" );
			vHead->setManuallyControlled( true );
			static Ogre::Quaternion vTemp( 7.07f, -1.5f, -1.4, -1.23f );
	//		vTemp.FromAngleAxis( Ogre::Radian( M_PI / 2 ), Ogre::Vector3( 0, 0, 1 ) );
			Ogre::Quaternion vThumb = iRotation;
			static float vScale = 1.0f;
			vThumb.x *= vScale;
			vHead->setOrientation( vThumb * vTemp );
		}
		break;
	case EFFECTOR_RIGHT_POINT:
		{
			Ogre::Bone* vHead;
			vHead = mSkeleton->getBone( "PinkyRight" );
			vHead->setManuallyControlled( true );
			vHead->setOrientation( iRotation );
			vHead = mSkeleton->getBone( "IndexRight" );
			vHead->setManuallyControlled( true );
			Ogre::Quaternion vTemp;
			vTemp = Ogre::Quaternion::IDENTITY;
			vHead->setOrientation( vTemp );
			vHead = mSkeleton->getBone( "ThumbRight" );
			vHead->setManuallyControlled( true );
			vTemp = Ogre::Quaternion( 7.07f, -1.5f, -1.4, -1.23f );
			Ogre::Quaternion vThumb = iRotation;
			static float vScale = 1.0f;
			vThumb.x *= vScale;
			vHead->setOrientation( vThumb * vTemp );
		}
		break;
	}
}

/************************************************************************/
const Ogre::Quaternion Loom::Avatar::cOgreAvatar::GetEffectorRotation( const eEffector iEffector )
/************************************************************************/
{
	Ogre::Bone* vNeck = mSkeleton->getBone( "Head" );

	Ogre::Quaternion vResult = vNeck->getOrientation();

	Ogre::Matrix3 vMatrix;
	mCalibration.extract3x3Matrix( vMatrix );
	Ogre::Quaternion vTransform( vMatrix.Inverse() );

	return vResult * vTransform;
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnSetEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation )
/************************************************************************/
{
	if ( iId != mId ) return;

	SetEffectorRotation( iEffector, iRotation );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetScale( const Ogre::Vector3 &iScale )
/************************************************************************/
{
	mNode->setScale( iScale );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnGetEffectorPosition( const size_t iId, const eEffector iEffector, Ogre::Vector3 &oPosition )
/************************************************************************/
{
	if ( iId != mId ) return;

	oPosition = GetEffectorPosition( iEffector );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnGetEffectorRotation( const size_t iId, const eEffector iEffector, Ogre::Quaternion &oRotation )
/************************************************************************/
{
	if ( iId != mId ) return;

	oRotation = GetEffectorRotation( iEffector );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetPosition( const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	mNode->setPosition( iPosition );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetRotation( const Ogre::Quaternion &iRotation )
/************************************************************************/
{
	mNode->setOrientation( iRotation );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetController( IController *iController )
/************************************************************************/
{
	mController = iController;
	mController->Init( this );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::ResetBones( void )
/************************************************************************/
{
	mSkeleton->reset(false);
}

/************************************************************************/
const Ogre::Vector3 & Loom::Avatar::cOgreAvatar::GetPosition( void ) const
/************************************************************************/
{
//	return mSkeleton->getRootBone()->getPosition();
	return mNode->getPosition();
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::UpdateAttachments()
/************************************************************************/
{
	// TODO: Cache effector positions/rotations

//	if ( !mController )
	{
	for ( size_t e=0; e<EFFECTOR_MAX; e++ )
	{
		if ( !mGrab[e] ) continue;

		cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
		cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
		cQScene *vQScene = vQMainWindow->GetScene();

		Ogre::Vector3 vPos = GetWorldEffectorPosition( (eEffector)e );

//		cArray<Ogre::String> vEntities;
		size_t vNumEntities;
		vQScene->OnGetEntitiesNoStd( NULL, vNumEntities );

		cQOgre::LPOGRESTR *vEntities;
		vEntities = new cQOgre::LPOGRESTR[ vNumEntities ];
		vQScene->OnGetEntitiesNoStd( vEntities, vNumEntities );
		for ( size_t i=0; i<vNumEntities; i++ )
		{
			Ogre::Vector3 vEntityPos;
			vQScene->OnGetPosition( *vEntities[i], vEntityPos );
//			if ( ( vEntityPos - vPos ).length() < 4.0f )
			vPos.y = vEntityPos.y;
			if ( ( vEntityPos - vPos ).length() < 10.0f )
			{
//				Ogre::Quaternion vRot;
//				vRot.FromAngleAxis( Ogre::Radian( M_PI * 0.95f ), Ogre::Vector3( 1, 0, 0 ) );
//				SetEffectorRotation( EFFECTOR_RIGHT_GRAB, vRot );

				OnAttach( mId, vEntities[i]->c_str(), (eEffector)e );
				break;
			}
		}
		if ( vNumEntities > 0 )
		{
			delete [] vEntities;
		}

		mGrab[e] = false;
	}
	}

	for ( size_t i=0; i<mAttachments.size(); i++ )
	{
		const eEffector vEffector = mAttachments[i].Effector;

		Ogre::Matrix4 vTrans;
		vTrans = mNode->_getFullTransform();
//		vTrans = Ogre::Matrix4::IDENTITY;

		Ogre::Vector3    vPos = GetEffectorPosition( vEffector );
//		Ogre::Quaternion vRot = GetEffectorRotation( vEffector ) * vTrans.extractQuaternion();

		Ogre::Quaternion vRot = mSkeleton->getBone( "HandRight" )->_getDerivedOrientation();
		Ogre::Vector3 vX, vY, vZ;
		vRot.ToAxes( vX, vY, vZ );
		static float vXOffset = 10.6;	//10;
		static float vYOffset = 11.2;	//5;
		static float vZOffset = 2.2;		//3
		vPos -= -vY * vXOffset;
		vPos += -vZ * vYOffset;
		vPos -= -vX * vZOffset;

		vPos = vTrans * vPos;
		vRot = vRot * vTrans.extractQuaternion() * mAttachments[i].Orientation;

		mAttachments[i].Entity->getParentNode()->setPosition( vPos );
		mAttachments[i].Entity->getParentNode()->setOrientation( vRot );
	}
}

/************************************************************************/
bool Loom::Avatar::cOgreAvatar::Attach( Ogre::Entity *iEntity, const eEffector iEffector )
/************************************************************************/
{
	for ( size_t i=0; i<mAttachments.size(); i++ )
	{
		if ( mAttachments[i].Entity == iEntity ) return mAttachments[i].Effector == iEffector;
	}

	Ogre::Quaternion vRot = ( mSkeleton->getBone( "HandRight" )->_getDerivedOrientation() * mNode->_getFullTransform().extractQuaternion() ).Inverse();
	mAttachments.push_back( sAttachment( iEntity, iEffector, vRot ) );

//	Dispatch( &IAvatarListenerEvent::OnGrab, this, iEffector, iEntity->getName() );
	size_t vLength = iEntity->getName().size() + 1;
	char *vParam = new char[ vLength + sizeof( eEffector ) + sizeof( cOgreAvatar* ) ];
	*(cOgreAvatar**)( vParam ) = this;
	*(eEffector*)( vParam + sizeof( cOgreAvatar* ) ) = iEffector;
	strcpy_s( vParam + sizeof( cOgreAvatar* ) + sizeof( eEffector ), vLength, iEntity->getName().c_str() );
	cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderListenerGrab"), cDispatcherHub::IParam( vParam ) );

	return true;
}

/************************************************************************/
bool Loom::Avatar::cOgreAvatar::Detach( Ogre::Entity *iEntity )
/************************************************************************/
{
	for ( size_t i=0; i<mAttachments.size(); i++ )
	{
		if ( mAttachments[i].Entity == iEntity )
		{
			mAttachments[i].Entity->getParentNode()->setOrientation( Ogre::Quaternion::IDENTITY );
//			Dispatch( &IAvatarListenerEvent::OnRelease, this, mAttachments[i].Effector, iEntity->getName() );
			cAvatarResponderListenerRelease::cParam vParam;
			vParam.Name = iEntity->getName();
			vParam.Effector = mAttachments[i].Effector;
			vParam.Id = mId;
			cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderListenerRelease"), cDispatcherHub::IParam( &vParam ) );
			mAttachments.erase( mAttachments.begin() + i );
			return true;
		}
	}

	return false;
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnAttach( const size_t iId, const Ogre::String &iEntityName, const eEffector iEffector )
/************************************************************************/
{
	if ( iId != mId ) return;

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();
	Ogre::Entity *vEntity = vScene->getEntity( iEntityName );

	if ( !vEntity )
	{
		TCHAR vTemp[ 256 ];
		StringCchPrintf( vTemp, 256, _T("Unknown entity: %S"), iEntityName );	// TODO: Use %s if not in unicode
		cLogger::Get().Log( cLogger::LOG_WARNING, _T("Global"), vTemp );
		return;
	}

	Attach( vEntity, iEffector );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnDetach( const size_t iId, const Ogre::String &iEntityName )
/************************************************************************/
{
	if ( iId != mId ) return;

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();
	Ogre::Entity *vEntity = vScene->getEntity( iEntityName );

	if ( !vEntity )
	{
		TCHAR vTemp[ 256 ];
		StringCchPrintf( vTemp, 256, _T("Unknown entity: %S"), iEntityName );	// TODO: Use %s if not in unicode
		cLogger::Get().Log( cLogger::LOG_WARNING, _T("Global"), vTemp );
		return;
	}

	Detach( vEntity );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::UpdateCamera()
/************************************************************************/
{
	if ( !mFPSCamera ) return;

	Ogre::Bone *vBone = mSkeleton->getBone("Head");
	Ogre::Vector3 vPos = mNode->_getFullTransform() * vBone->_getDerivedPosition();

	Ogre::Quaternion vRot = vBone->_getDerivedOrientation(); // * mNode->_getFullTransform().extractQuaternion();
	vRot = GetEffectorRotation( EFFECTOR_HEAD );

	Ogre::Vector3 vX;
	Ogre::Vector3 vY;
	Ogre::Vector3 vZ;
	vRot.ToAxes( vX, vY, vZ );

	static float gCameraDistance = 16.0f;
	if ( vPos.z > -60.0f )
	{
		vPos += vZ * -gCameraDistance;
	}
	else
	{
		vPos += vZ *  gCameraDistance;
	}

	if ( ( GetAsyncKeyState( 'T' ) & 0x8000 ) ) gCameraDistance += 0.02f;
	if ( ( GetAsyncKeyState( 'G' ) & 0x8000 ) ) gCameraDistance -= 0.02f;

//	vX = -vX; vY = -vY; vZ = -vZ;
//	vRot.FromAxes( vX, vY, vZ );

	static float gFOV = 80.0f;
	mFPSCamera->setPosition( vPos );
	mFPSCamera->setFOVy( Ogre::Radian( gFOV * M_PI / 180.0f ) );
//	mFPSCamera->lookAt( Ogre::Vector3( 0, -18, -25 ) );
//	mFPSCamera->lookAt( Ogre::Vector3( 0, -5, -25 ) );
	if ( vPos.z > -60.0f )
	{
		mFPSCamera->lookAt( Ogre::Vector3( 0, -10, -120 ) );
	}
	else
	{
		mFPSCamera->lookAt( Ogre::Vector3( 0, -10,    0 ) );
	}

	if ( ( GetAsyncKeyState( 'Y' ) & 0x8000 ) ) gFOV += 0.1f;
	if ( ( GetAsyncKeyState( 'H' ) & 0x8000 ) ) gFOV -= 0.1f;

//	mFPSCamera->setOrientation( vRot );
}

/************************************************************************/
const Ogre::Vector3 Loom::Avatar::cOgreAvatar::GetWorldEffectorPosition( const eEffector iEffector )
/************************************************************************/
{

	Ogre::Quaternion vRot = mSkeleton->getBone( "HandRight" )->_getDerivedOrientation();
	Ogre::Vector3 vX, vY, vZ;
	vRot.ToAxes( vX, vY, vZ );
	static float vXOffset = 10.6;	//10;
	static float vYOffset = 11.2;	//5;
	static float vZOffset = 2.2;		//3
	Ogre::Vector3 vPos = GetEffectorPosition( iEffector ) + vY * vXOffset;
	vPos -= vZ * vYOffset;
	vPos += vX * vZOffset;

	return mNode->_getFullTransform() * vPos;

}

#pragma warning( disable: 4189 )
/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnGrab( const size_t iId, const eEffector iEffector )
/************************************************************************/
{
	if ( iId != mId ) return;

	mGrab[ iEffector ] = true;
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnRelease( const size_t iId, const eEffector iEffector )
/************************************************************************/
{
	if ( iId != mId ) return;

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();

	for ( size_t i=0; i<mAttachments.size(); i++ )
	{
		if ( mAttachments[i].Effector == iEffector )
		{
			Ogre::Vector3 vPos;
			vQScene->OnGetPosition( mAttachments[i].Entity->getName(), vPos );
			vPos.y = 0.0f;
			vQScene->OnSetPosition( mAttachments[i].Entity->getName(), vPos );

//			Dispatch( &IAvatarListenerEvent::OnRelease, this, mAttachments[i].Effector, mAttachments[i].Entity->getName() );
			cAvatarResponderListenerRelease::cParam vParam;
			vParam.Name = mAttachments[i].Entity->getName();
			vParam.Effector = mAttachments[i].Effector;
			vParam.Id = mId;
			cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderListenerRelease"), cDispatcherHub::IParam( &vParam ) );
			mAttachments.erase( mAttachments.begin() + i );
			i--;
		}
	}
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SolveLeft( void )
/************************************************************************/
{
	mIKChainLeft->Solve( mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
}

/************************************************************************/
void cOgreAvatar::UpdatePointing() 
/************************************************************************/
{
	Ogre::SubMesh *vSub = mLines->getSubMesh( 0 );
	vSub->vertexData->vertexCount = 0;
	vSub->indexData->indexCount = 0;

	// Fill vertices
	Ogre::HardwareVertexBufferSharedPtr vVertices = vSub->vertexData->vertexBufferBinding->getBuffer(0);

	struct sVertex
	{
		Ogre::Vector3 Position;
	};

	Ogre::Bone *vBone = mSkeleton->getBone( "HandRight" );

	Ogre::Matrix4 vTransform = mNode->_getFullTransform();
	Ogre::Quaternion vTransformRot( vTransform.extractQuaternion() );

	sVertex *vVtx = (sVertex*)( vVertices->lock( vVertices->getVertexSize() * vSub->vertexData->vertexCount, vVertices->getVertexSize() * 2, Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
	Ogre::Vector3 vHand = vTransform * vBone->_getDerivedPosition();
	Ogre::Quaternion vOrientation = vTransformRot * vBone->_getDerivedOrientation();
	Ogre::Vector3 vDir = vOrientation.yAxis();
	vDir.normalise();
	if ( vDir.y < 0 )
	{
		vDir *= vHand.y / -vDir.y;
	}
	else
	{
		vDir *= 10;
	}
	vVtx->Position = vHand; vVtx++;
	vVtx->Position = vHand + vDir; vVtx++;
	vVertices->unlock();

	Ogre::HardwareIndexBufferSharedPtr vIndices = vSub->indexData->indexBuffer;
	Ogre::ulong *vIdx = (Ogre::ulong*)( vIndices->lock( vSub->indexData->indexCount * sizeof( Ogre::ulong ), sizeof( Ogre::ulong ) * 2, Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
	*vIdx++ = 0;
	*vIdx++ = 1;
	vIndices->unlock();

	vSub->vertexData->vertexCount = 2;
	vSub->indexData->indexCount   = 2;
}

/************************************************************************/
void cOgreAvatar::OnRender( void )
/************************************************************************/
{
	UpdateCamera();
	UpdateAttachments();
	UpdatePointing();

	if ( !mController )
	{
//		SetEffectorPosition( EFFECTOR_LEFT_HAND, mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
		SetEffectorPosition( EFFECTOR_RIGHT_HAND, mEffectorPositions[ EFFECTOR_RIGHT_HAND ] );
		return;
	}

	mController->Update();
}

/************************************************************************/
void cOgreAvatar::CreateLines( void )
/************************************************************************/
{
	char vTemp[ 256 ];
	sprintf_s( vTemp, 256, "Avatar%d.Line", mId );
	mLines = Ogre::MeshManager::getSingleton().createManual( vTemp, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

	// Create mesh for textured lines
	Ogre::SubMesh *vSub = mLines->createSubMesh();
	vSub->operationType = Ogre::RenderOperation::OT_LINE_LIST;
	vSub->setMaterialName( "Avatar/Line" );
	vSub->useSharedVertices = false;
	vSub->vertexData = new Ogre::VertexData;
	vSub->indexData  = new Ogre::IndexData;
	vSub->vertexData->vertexDeclaration->addElement( 0,  0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
	/*
	vSub->vertexData->vertexDeclaration->addElement( 0, 12, Ogre::VET_FLOAT3, Ogre::VES_TEXTURE_COORDINATES, 1 );
	vSub->vertexData->vertexDeclaration->addElement( 0, 24, Ogre::VET_FLOAT4, Ogre::VES_TEXTURE_COORDINATES, 0 );
	*/

	//    const int vMaxNumLines = 30000 * 5;
	const int vMaxNumLines = 1;
	Ogre::HardwareVertexBufferSharedPtr vVertices = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vSub->vertexData->vertexDeclaration->getVertexSize( 0 ), vMaxNumLines * 2, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
	Ogre::HardwareIndexBufferSharedPtr  vIndices  = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer ( Ogre::HardwareIndexBuffer::IT_32BIT, vMaxNumLines * 2 * sizeof( Ogre::ulong ), Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );

	vSub->vertexData->vertexBufferBinding->setBinding( 0, vVertices );
	vSub->vertexData->vertexStart = 0;
	vSub->vertexData->vertexCount = 0;

	vSub->indexData->indexBuffer = vIndices;

	mLines->_setBounds( Ogre::AxisAlignedBox( -200, -200, -200, 200, 200, 200 ) );
	mLines->_setBoundingSphereRadius( 400 );

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	char vTempEntity[ 256 ];
	sprintf_s( vTempEntity, 256, "Avatar%d.Line", mId );
	Ogre::Entity *vSpewsEntity = vScene->createEntity( vTempEntity, vTemp );
	Ogre::SceneNode *vNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 0, 0 ) );
	vNode->attachObject( vSpewsEntity );
}

