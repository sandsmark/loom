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

/************************************************************************/
cOgreAvatar::cOgreAvatar( const Ogre::String &iName )
: mController( NULL )
/************************************************************************/
{
	CreateEntity( iName );
	CreateIkChain();

	// Subscribe for Avatar messages
	cAvatarResponderSetEffectorPosition::Get().AddListener( *this );
	cAvatarResponderSetEffectorRotation::Get().AddListener( *this );
	Speech::cSpeechResponderHeard::Get().AddListener( *this );

	// Subscribe for Ogre rendering message
	cOgreResponderOnRender::Get().AddListener( *this );

	SetEffectorPosition( EFFECTOR_LEFT_HAND , Ogre::Vector3( -100, 100, 0 ) );
	SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3(  100, 100, 0 ) );
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
//	mNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 100, 340, -100 ) );
	Ogre::Quaternion vRotation;
	vRotation.FromAngleAxis( Ogre::Radian( Ogre::Math::PI ), Ogre::Vector3( 0, 1, 0 ) );
	mNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 0, 0 ), vRotation );
	mNode->setScale( 0.5f, 0.5f, 0.5f );
	mEntity->setMaterialName( "Avatar/Humanoid" );
	mEntity->setCastShadows( true );
	mEntity->setDisplaySkeleton( false );

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
		// Lower Arm
		vBase = mIKChainLeft->GetJointAtDepth(5); //la
		vBase->SetMinJointAngleX(0);
		vBase->SetMaxJointAngleX(0);
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
		vBase->SetWeight(0.1);
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
	}
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPos )
/************************************************************************/
{
	Ogre::Vector4 vPos( iPos.x, iPos.y, iPos.z, 1 );
	vPos = vPos * mCalibration;
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
		mIKChainLeft->Solve( mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
		break;
	case EFFECTOR_RIGHT_HAND:
//		mIKChainLeft->Solve( mEffectorPositions[ EFFECTOR_LEFT_HAND ] );
		mIKChainRight->Solve( mEffectorPositions[ EFFECTOR_RIGHT_HAND ] );
		break;
	}

}

/************************************************************************/
const Ogre::Vector3 Loom::Avatar::cOgreAvatar::GetEffectorPosition( const eEffector iEffector )
/************************************************************************/
{
	return mIKChainLeft->GetEffectorPosition();
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnSetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	SetEffectorPosition( iEffector, iPosition );
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

			while ( !vInterface->getNextEntryConst( vLeftArmSensorId, vX, vY, vZ, vA, vE, vR, 1000 ) );
			vLeft = Ogre::Vector3( vX, vY, vZ );
			while ( !vInterface->getNextEntryConst( vRightArmSensorId, vX, vY, vZ, vA, vE, vR, 1000 ) );
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

			while ( !vInterface->getNextEntryConst( vLeftArmSensorId, vX, vY, vZ, vA, vE, vR, 1000 ) );
			vLeft = Ogre::Vector3( vX, vY, vZ );
			while ( !vInterface->getNextEntryConst( vRightArmSensorId, vX, vY, vZ, vA, vE, vR, 1000 ) );
			vRight = Ogre::Vector3( vX, vY, vZ );
			while ( !vInterface->getNextEntryConst( vHeadSensorId, vX, vY, vZ, vA, vE, vR, 1000 ) );
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

/************************************************************************/
const Ogre::Quaternion Loom::Avatar::cOgreAvatar::GetEffectorRotation( const eEffector iEffector )
/************************************************************************/
{
	Ogre::Bone* vNeck = mSkeleton->getBone( "Neck" );

	return vNeck->getOrientation();
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnSetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation )
/************************************************************************/
{
	SetEffectorRotation( iEffector, iRotation );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetScale( const Ogre::Vector3 &iScale )
/************************************************************************/
{
	mNode->setScale( iScale );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnGetEffectorPosition( const eEffector iEffector, Ogre::Vector3 &oPosition )
/************************************************************************/
{
	oPosition = GetEffectorPosition( iEffector );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnGetEffectorRotation( const eEffector iEffector, Ogre::Quaternion &oRotation )
/************************************************************************/
{
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
void cOgreAvatar::OnRender( void )
/************************************************************************/
{
	if ( !mController ) return;

	mController->Update();
}
