#include "StdAfx.h"
#include <Avatar/Controller/cControllerNetwork.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Ogre/OgreVector3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "OgreApp/Module/cModuleOgreApp.h"
#include "../../../OgreApp/src/Qt/MainWindow/cQMainWindow.h"
#include "../../../OgreApp/src/Qt/Ogre/cQScene.h"
#include <Core/Module/cModuleManager.h>
#include <Ogre/OgreSceneNode.h>
#include <Avatar/Event/Responders/cAvatarResponderNotifyBones.h>
#include <Ogre/OgreSkeletonInstance.h>
#include <Ogre/OgreBone.h>

using namespace Loom::Avatar;

using Loom::Core::cModuleManager;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQScene;

/************************************************************************/
cControllerNetwork::cControllerNetwork()
: mAvatar( NULL )
/************************************************************************/
{
	mActive = true;
}

/************************************************************************/
cControllerNetwork::~cControllerNetwork()
/************************************************************************/
{
}

/************************************************************************/
void cControllerNetwork::Init( cOgreAvatar *iAvatar )
/************************************************************************/
{
	ASSERTTXT( mAvatar == NULL, _T( "Avatar to control is already set!" ) );

	mAvatar = iAvatar;

	cAvatarResponderListenerNotifyBones::Get().AddListener( *this );
}

/************************************************************************/
void cControllerNetwork::Update()
/************************************************************************/
{
	if ( !mActive ) return;
}

/************************************************************************/
void Loom::Avatar::cControllerNetwork::SetActive( bool iActive )
/************************************************************************/
{
	if ( mActive == iActive ) return;
	mActive = iActive;
}

/************************************************************************/
void Loom::Avatar::cControllerNetwork::OnNotifyEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	/*
	if ( !mActive ) return;
	if ( !mAvatar ) return;

	if ( mAvatar->GetId() != iId ) return;

	Ogre::Vector3 vPosition = mAvatar->GetNode()->_getFullTransform().inverse() * iPosition; 
	mAvatar->SetEffectorPosition( iEffector, vPosition );
	*/
}

/************************************************************************/
void Loom::Avatar::cControllerNetwork::OnNotifyEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation )
/************************************************************************/
{

}

/************************************************************************/
void Loom::Avatar::cControllerNetwork::OnNotifyGrab( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName )
/************************************************************************/
{

}

/************************************************************************/
void Loom::Avatar::cControllerNetwork::OnNotifyRelease( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName )
/************************************************************************/
{

}

/************************************************************************/
void Loom::Avatar::cControllerNetwork::OnNotifyBones( const size_t iId, const sBone *iBones, const size_t iNumBones )
/************************************************************************/
{
	if ( !mActive ) return;
	if ( !mAvatar ) return;
	if ( mAvatar->GetId() != iId ) return;

	Ogre::SkeletonInstance *vSkeleton = mAvatar->GetSkeleton();
	for ( size_t i=0; i<vSkeleton->getNumBones(); i++ )
	{
		Ogre::Bone* vBone = vSkeleton->getBone(i);
		vBone->setManuallyControlled( true );
		vBone->setPosition( iBones[i].Position );
		vBone->setOrientation( iBones[i].Orientation );
	}
}
