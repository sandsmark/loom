#pragma once

namespace Ogre
{
	class Vector3;
	class Quaternion;
}

BEGIN_NAMESPACE( Avatar )

class IAvatarEvent
{
public:
	virtual void OnSetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPosition ) = 0;
	virtual void OnSetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation ) = 0;
};

END_NAMESPACE()
