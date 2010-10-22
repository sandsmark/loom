#pragma once
#include <Core\Event\cDispatcherHub.h>
#include <Ogre/OgreString.h>

namespace Ogre
{
	class ColourValue;
	class Vector3;
};

BEGIN_NAMESPACE( Weaver )

class IOgreEvent
{
public:
	virtual void OnSetBackgroundColour( const Ogre::ColourValue &iColour ) = 0;
	virtual void OnCreateBox( const Ogre::String &iName, const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iSize ) = 0;
	virtual void OnSetPosition( const Ogre::String &iName, const Ogre::Vector3 &iPosition ) = 0;
};

END_NAMESPACE()