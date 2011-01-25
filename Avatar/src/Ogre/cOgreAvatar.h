#pragma once

#include <Avatar/Event/IAvatarEvent.h>
#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Speech/Event/cSpeechResponders.h>
#include <Ogre/OgreMatrix3.h>
#include <Ogre/OgreMatrix4.h>

using Loom::OgreApp::IOgreListenerEvent;

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

class IController;

class AVATAR_API cOgreAvatar : public IAvatarEvent, public IOgreListenerEvent, public Speech::ISpeechListenerEvent
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
	IKChain* mIKChainLeft;
	IKChain* mIKChainRight;
	Ogre::Vector3 mEffectorPositions[ EFFECTOR_MAX ];
	IController *mController;

	Ogre::Matrix4 mCalibration;

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

	void SetController( IController *iController );
	void ResetBones( void );
	const Ogre::Vector3 &GetPosition( void ) const;

	// IAvatarEvent
	virtual void OnGetEffectorPosition( const eEffector iEffector, Ogre::Vector3 &oPosition );
	virtual void OnGetEffectorRotation( const eEffector iEffector, Ogre::Quaternion &oRotation );
	virtual void OnSetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPosition );
	virtual void OnSetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation );

	// ISpeechListenerEvent
	virtual void OnHeard( const std::wstring &text );

	// IOgreListenerEvent methods
	virtual void OnRender( void );

};

END_NAMESPACE()
