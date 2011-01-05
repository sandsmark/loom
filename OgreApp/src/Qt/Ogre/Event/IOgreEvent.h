#pragma once
#include <Core\Event\cDispatcherHub.h>
#include <Ogre/OgreString.h>
#include <Ogre/OgreColourValue.h>

namespace Ogre
{
	class Vector3;
};

BEGIN_NAMESPACE( OgreApp )

class IOgreEvent
{
public:
	virtual void OnSetBackgroundColour( const Ogre::ColourValue &iColour ) = 0;
	virtual void OnCreateBox( const Ogre::String &iName, const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iSize ) = 0;
	virtual void OnSetPosition( const Ogre::String &iName, const Ogre::Vector3 &iPosition ) = 0;
	virtual void OnGetPosition( const Ogre::String &iName, Ogre::Vector3 &oPosition ) = 0;
	virtual void OnGetBoundingBox( const Ogre::String &iName, Ogre::AxisAlignedBox &oBounds ) = 0;
	virtual void OnMoveTo( const Ogre::String &iName, const Ogre::Vector3 &iPosition, float iSpeed ) = 0;
	virtual void OnSetTexture( const Ogre::String &iName, void *iTextureData, unsigned long iSize ) = 0;
	virtual void OnGetEntities( cArray<Ogre::String> &oNames ) = 0;
	virtual void OnDebugLog( const cString &iText ) = 0;
	virtual void OnOutput( const cString &iText ) = 0;
	virtual void OnClearLines( void ) = 0;
	virtual void OnAddLine( const Ogre::Vector3 &iFrom, const Ogre::Vector3 &iTo, const Ogre::ColourValue &Colour = Ogre::ColourValue::White ) = 0;
};

END_NAMESPACE()