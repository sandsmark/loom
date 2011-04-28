#pragma once

#include <Ogre/OgreString.h>
#include <Ogre/OgreVector3.h>
#include <Ogre/OgreQuaternion.h>

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class IAvatarListenerEvent
{
public:
	struct sBone
	{
		Ogre::Vector3       Position;
		Ogre::Quaternion Orientation;
	};

public:
	virtual void OnNotifyEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition ) = 0;
	virtual void OnNotifyEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation ) = 0;
	virtual void OnNotifyGrab( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName ) = 0;
	virtual void OnNotifyRelease( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName ) = 0;
	virtual void OnNotifyBones( const size_t iId, const sBone *iBones, const size_t iNumBones ) = 0;
};

END_NAMESPACE()
