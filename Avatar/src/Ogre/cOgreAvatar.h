#pragma once

#include <Avatar/Event/IAvatarEvent.h>
#include <Ogre/OgreMatrix3.h>

namespace Ogre
{
	class Entity;
	class SkeletonInstance;
	class SceneNode;
	class Vector3;
	class Matrix3;
	class Quaternion;
};

class IKChain;

BEGIN_NAMESPACE( Avatar )

class AVATAR_API cOgreAvatar : public IAvatarEvent
{
protected:
	enum eCalibrationState
	{
		CALIBRATION_STATE1 = 0,
		CALIBRATION_STATE2,
		CALIBRATION_STATE3,
		CALIBRATION_DONE,
	};
	eCalibrationState mCalibrationState;

protected:
	Ogre::Entity *mEntity;
	Ogre::SkeletonInstance *mSkeleton;
	Ogre::SceneNode *mNode;
	IKChain* mIKChain;

	void CreateEntity( const Ogre::String &iName );
	void CreateIkChain();
	void UpdateMatrices();

public:
	cOgreAvatar( const Ogre::String &iName );

	void SetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPos );
	const Ogre::Vector3 GetEffectorPosition( const eEffector iEffector );
	void SetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation );
	const Ogre::Quaternion GetEffectorRotation( const eEffector iEffector );

	void Calibrate( void );
	void SetPosition( const Ogre::Vector3 &iPosition );
	void SetRotation( const Ogre::Quaternion &iRotation );
	void SetScale( const Ogre::Vector3 &iScale );

	// IAvatarEvent
	virtual void OnGetEffectorPosition( const eEffector iEffector, Ogre::Vector3 &oPosition );
	virtual void OnGetEffectorRotation( const eEffector iEffector, Ogre::Quaternion &oRotation );
	virtual void OnSetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPosition );
	virtual void OnSetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation );

	// ISpeechListenerEvent
	virtual void OnHeard( const std::wstring &text );

};

END_NAMESPACE()
