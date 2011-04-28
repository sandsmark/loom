#pragma once

#include <Ogre/OgreString.h>

namespace Ogre
{
	class Vector3;
	class Quaternion;
}

BEGIN_NAMESPACE( Avatar )

class IAvatarEvent
{
public:
	virtual void OnGetEffectorPosition( const size_t iId, const eEffector iEffector, Ogre::Vector3 &iPosition ) = 0;
	virtual void OnGetEffectorRotation( const size_t iId, const eEffector iEffector, Ogre::Quaternion &iRotation ) = 0;
	virtual void OnSetEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition ) = 0;
	virtual void OnSetEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation ) = 0;
	virtual void OnAttach( const size_t iId, const Ogre::String &iEntityName, const eEffector iEffector ) = 0;
	virtual void OnDetach( const size_t iId, const Ogre::String &iEntityName ) = 0;
	virtual void OnGrab( const size_t iId, const eEffector iEffector ) = 0;
	virtual void OnRelease( const size_t iId, const eEffector iEffector ) = 0;
};

END_NAMESPACE()
