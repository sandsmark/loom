#pragma once

#include <Ogre/OgreVector3.h>

using Loom::Core::cObject;

BEGIN_NAMESPACE( Avatar )

class cConfig : public cObject<cConfig>
{
public:
	bool  SingleAvatar;
	float Orientation;
	Ogre::Vector3 Position;

	cConfig();
};

END_NAMESPACE()
