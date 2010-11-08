#include <Avatar/Ogre/cOgreAvatar.h>
#include <Avatar/IKChain/IKChain.h>
#include <Avatar/IKChain/IKJoint.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreSkeletonInstance.h>
#include <Ogre/OgreEntity.h>
#include <Ogre/OgreLight.h>
#include <Ogre/OgreQuaternion.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Avatar/IKChain/IKChain.h>
#include <Avatar/Event/cAvatarResponder.h>

using namespace Loom::Avatar;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQOgre;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;
using Loom::Avatar::cAvatarResponderSetEffectorPosition;

/************************************************************************/
cOgreAvatar::cOgreAvatar()
/************************************************************************/
{
	CreateEntity();
	CreateIkChain();

	// Subscribe for Avatar messages
	cAvatarResponderSetEffectorPosition::Get().AddListener( *this );

	SetEffectorPosition( Ogre::Vector3( 0, -100, 0 ) );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::CreateEntity()
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	// Create spotlight
	Ogre::Light* vLightSpot1;
	vLightSpot1 = vScene->createLight("SpotLight1");
	vLightSpot1->setType(Ogre::Light::LT_SPOTLIGHT);
	//	vLightSpot1->setPosition(-900, 750, 375);
	vLightSpot1->setDirection(0.79, -0.38, 0.48);
	vLightSpot1->setPosition(-790, 380, -480);
	vLightSpot1->setDiffuseColour(0.5, 0.5, 0.5);

	// Create humanoid
	mEntity = vScene->createEntity( "SuperHumanoid", "models/SuperHumanoid.mesh" );
//	mNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 100, 340, -100 ) );
	Ogre::Quaternion vRotation;
	vRotation.FromAngleAxis( Ogre::Radian( Ogre::Math::PI ), Ogre::Vector3( 0, 1, 0 ) );
	mNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 0, 0 ), vRotation );
	mNode->setScale( 0.5f, 0.5f, 0.5f );
	mEntity->setMaterialName( "Avatar/Humanoid" );
	mEntity->setCastShadows( true );
	mEntity->setDisplaySkeleton( true );

	mNode->attachObject( mEntity );
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::CreateIkChain()
/************************************************************************/
{
	// Create IK chain manually
	mIKChain = new IKChain(mNode);
	Ogre::SkeletonInstance* vSkel = mEntity->getSkeleton();
	Ogre::Bone* vBone = vSkel->getBone("TailBone");
	mIKChain->PushOgreBone(vBone);
	vBone = vSkel->getBone("Spine_1");
	mIKChain->PushOgreBone(vBone);
	vBone = vSkel->getBone("Spine_2");
	mIKChain->PushOgreBone(vBone);
	vBone = vSkel->getBone("ShoulderLeft");
	mIKChain->PushOgreBone(vBone);
	vBone = vSkel->getBone("ArmUpperLeft");
	mIKChain->PushOgreBone(vBone);
	vBone = vSkel->getBone("ArmLowerLeft");
	mIKChain->PushOgreBone(vBone);
	vBone = vSkel->getBone("HandLeft");
	mIKChain->PushOgreBone(vBone);

	// Lower Spine
	IKJoint* vBase = mIKChain->GetJointAtDepth(1);
	vBase->SetJointAngleSymmetric(Ogre::Math::PI/16);
	vBase->SetWeight(0.1);
	// Upper Spine
	vBase = mIKChain->GetJointAtDepth(2); 
	vBase->SetJointAngleSymmetric(Ogre::Math::PI/16);
	vBase->SetWeight(0.1);
	// Shoulder
	vBase = mIKChain->GetJointAtDepth(3); //rs
	vBase->SetJointAngleSymmetric(0);
	vBase->SetWeight(0.1);
	// Upper arm
	vBase = mIKChain->GetJointAtDepth(4); //ua
	vBase->SetMinJointAngleX(0);
	vBase->SetMaxJointAngleX(0);
	// Lower Arm
	vBase = mIKChain->GetJointAtDepth(5); //la
	vBase->SetMinJointAngleX(0);
	vBase->SetMaxJointAngleX(0);
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::SetEffectorPosition( const Ogre::Vector3 &iPos )
/************************************************************************/
{
	mIKChain->SolveForTargetZInv(iPos);
	mIKChain->UpdateOgreSkeletonZ();	
	mIKChain->SolveForTargetYInv(iPos);
	mIKChain->UpdateOgreSkeletonY();	
	mIKChain->SolveForTargetXInv(iPos);			
	mIKChain->UpdateOgreSkeletonX();	
}

/************************************************************************/
const Ogre::Vector3 Loom::Avatar::cOgreAvatar::GetEffectorPosition( void )
/************************************************************************/
{
	return mIKChain->GetEffectorPosition();
}

/************************************************************************/
void Loom::Avatar::cOgreAvatar::OnSetEffectorPosition( const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	SetEffectorPosition( iPosition );
}
