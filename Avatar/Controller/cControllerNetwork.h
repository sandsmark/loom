#pragma once

#include <Avatar/Controller/IController.h>
#include <windows.h>	// DWORD
#include <Ogre/OgreVector3.h>
#include <Core/Spline/cMotion.h>
#include <Avatar/Event/IAvatarListenerEvent.h>

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API cControllerNetwork : public IController, public IAvatarListenerEvent
{
protected:
	cOgreAvatar *mAvatar;

public:
	cControllerNetwork();
	virtual ~cControllerNetwork();

	// IController methods
	virtual void Init( cOgreAvatar *iAvatar );
	virtual void Update( void );

	virtual void SetActive( bool iActive );

	// IAvatarListenerEvent methods
	virtual void OnNotifyEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition );
	virtual void OnNotifyEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation );
	virtual void OnNotifyGrab( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName );
	virtual void OnNotifyRelease( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName );
	virtual void OnNotifyBones( const size_t iId, const sBone *iBones, const size_t iNumBones );
};


END_NAMESPACE()
