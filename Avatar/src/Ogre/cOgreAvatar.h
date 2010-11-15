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
	Ogre::Matrix3 mCalibration;
	IKChain* mIKChain;

	void CreateEntity();
	void CreateIkChain();

public:
	cOgreAvatar();

	void SetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPos );
	const Ogre::Vector3 GetEffectorPosition( const eEffector iEffector );
	void SetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation );
	const Ogre::Quaternion GetEffectorRotation( const eEffector iEffector );

	void Calibrate( void );

	// IAvatarEvent
	virtual void OnSetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPosition );
	virtual void OnSetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation );

	// ISpeechListenerEvent
	virtual void OnHeard( const std::wstring &text );

};

END_NAMESPACE()
