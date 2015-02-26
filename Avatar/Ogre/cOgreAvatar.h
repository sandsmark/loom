#pragma once

#include <Avatar/Event/IAvatarEvent.h>
#include <Avatar/Event/IAvatarListenerEvent.h>
#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Speech/Event/cSpeechResponders.h>
#include <Ogre/OgreMatrix3.h>
#include <Ogre/OgreMatrix4.h>
#include <Ogre/OgreMesh.h>
#include <vector>

using Loom::OgreApp::IOgreListenerEvent;

namespace Ogre
{
	class Entity;
	class SkeletonInstance;
	class SceneNode;
	class Vector3;
	class Matrix3;
	class Quaternion;
	class Camera;
};

class IKChain;

BEGIN_NAMESPACE( Avatar )

class IController;

class AVATAR_API cOgreAvatar : public IAvatarEvent, public IOgreListenerEvent, public Speech::ISpeechListenerEvent, public Loom::Core::IEventDispatcher< IAvatarListenerEvent >
{
	friend class cControllerMouse;

public:
	enum eCalibrationState
	{
		CALIBRATION_STATE1 = 0,
		CALIBRATION_STATE2,
		CALIBRATION_STATE3,
		CALIBRATION_DONE,
	};
	eCalibrationState mCalibrationState;

	struct sAttachment
	{
		eEffector Effector;
		Ogre::Entity *Entity;
		Ogre::Quaternion Orientation;

		sAttachment() {}
		sAttachment( Ogre::Entity *iEntity, eEffector iEffector, const Ogre::Quaternion iOrientation ) : Entity( iEntity ), Effector( iEffector ), Orientation( iOrientation )  {}
	};
	// It's a vector, because iteration happens more often than remove
	std::vector<sAttachment> mAttachments;

	bool mGrab[ EFFECTOR_MAX ];

protected:
	Ogre::Entity *mEntity;
	Ogre::SkeletonInstance *mSkeleton;
	Ogre::SceneNode *mNode;
	IKChain* mIKChainLeft;
	IKChain* mIKChainRight;
	Ogre::Vector3 mEffectorPositions[ EFFECTOR_MAX ];
	IController *mController;
	size_t mId;

	Ogre::Matrix4 mCalibration;
	Ogre::Vector3 mMidPoint;
	float mScale;
	Ogre::Vector3 mTestLeft;
	Ogre::Vector3 mTestRight;

	Ogre::Camera *mFPSCamera;

	Ogre::MeshPtr mLines;
	bool mInteractive;

	void CreateEntity( const Ogre::String &iName );
	void CreateIkChain();
	void UpdateMatrices();
	void UpdateAttachments();
	void UpdateCamera();
	void UpdatePointing();

public:
	cOgreAvatar( const Ogre::String &iName, const size_t iId );

	void SetEffectorPosition( const eEffector iEffector, const Ogre::Vector3 &iPos );
	const Ogre::Vector3 GetEffectorPosition( const eEffector iEffector );
	void SetEffectorRotation( const eEffector iEffector, const Ogre::Quaternion &iRotation );
	const Ogre::Quaternion GetEffectorRotation( const eEffector iEffector );

	const Ogre::Vector3 GetWorldEffectorPosition( const eEffector iEffector );

	void Calibrate( void );
	void SetPosition( const Ogre::Vector3 &iPosition );
	void SetRotation( const Ogre::Quaternion &iRotation );
	void SetScale( const Ogre::Vector3 &iScale );

	void SetController( IController *iController );
	IController *GetController( void ) const { return mController; }
	void ResetBones( void );
	const Ogre::Vector3 &GetPosition( void ) const;

	bool Attach( Ogre::Entity *iEntity, const eEffector iEffector );
	bool Detach( Ogre::Entity *iEntity );

	void SetFPSCamera( Ogre::Camera *iCamera )
	{
		mFPSCamera = iCamera;
	}

	Ogre::SkeletonInstance *GetSkeleton( void ) const { return mSkeleton; }
	Ogre::SceneNode *GetNode( void ) const { return mNode; }
	Ogre::Entity *GetEntity( void ) const { return mEntity; }

	void SolveLeft( void );
	const size_t GetId( void ) const { return mId; }

	void CreateLines( void );

	// IAvatarEvent
	virtual void OnGetEffectorPosition( const size_t Id, const eEffector iEffector, Ogre::Vector3 &oPosition );
	virtual void OnGetEffectorRotation( const size_t iId, const eEffector iEffector, Ogre::Quaternion &oRotation );
	virtual void OnSetEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition );
	virtual void OnSetEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation );
	virtual void OnAttach( const size_t iId, const Ogre::String &iEntityName, const eEffector iEffector );
	virtual void OnDetach( const size_t iId, const Ogre::String &iEntityName );
	virtual void OnGrab( const size_t iId, const eEffector iEffector );
	virtual void OnRelease( const size_t iId, const eEffector iEffector );

	// ISpeechListenerEvent
	virtual void OnHeard( const std::wstring &text );

	// IOgreListenerEvent methods
	virtual void OnRender( void );

	void SetInteractive( const bool iInteractive )
	{
		mInteractive = iInteractive;
	}
	bool IsInteractive( void ) const { return mInteractive; }
};

END_NAMESPACE()
