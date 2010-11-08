#pragma once

namespace Ogre
{
	class Vector3;
}

BEGIN_NAMESPACE( Avatar )

class IAvatarEvent
{
public:
	virtual void OnSetEffectorPosition( const Ogre::Vector3 &iPosition ) = 0;
};

END_NAMESPACE()
