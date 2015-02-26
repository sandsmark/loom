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
	int Id1;
	int Id2;
	char *Controller1;
	char *Controller2;
	bool ViewportVisible1;
	bool ViewportVisible2;

	cConfig();
};

END_NAMESPACE()
