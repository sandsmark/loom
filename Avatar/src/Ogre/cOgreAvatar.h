#pragma once

#include <Avatar/Event/IAvatarEvent.h>

namespace Ogre
{
	class Entity;
	class SkeletonInstance;
	class SceneNode;
	class Vector3;
};

class IKChain;

BEGIN_NAMESPACE( Avatar )

class AVATAR_API cOgreAvatar : public IAvatarEvent
{
protected:
	Ogre::Entity *mEntity;
	Ogre::SkeletonInstance *mSkeleton;
	Ogre::SceneNode *mNode;
	IKChain* mIKChain;

	void CreateEntity();
	void CreateIkChain();

public:
	cOgreAvatar();

	void SetEffectorPosition( const Ogre::Vector3 &iPos );
	const Ogre::Vector3 GetEffectorPosition( void );

	// IAvatarEvent
	virtual void OnSetEffectorPosition( const Ogre::Vector3 &iPosition );
};

END_NAMESPACE()
