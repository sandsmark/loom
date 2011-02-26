#pragma once

#include <Core/Container/cArray.h>
#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Ogre/OgreVector2.h>
#include <Ogre/OgreVector3.h>
#include <Core/Spline/cMotion.h>

using Loom::OgreApp::IOgreListenerEvent;
using Loom::Core::cMotion;

namespace Ogre
{
	class Billboard;
};

using Loom::Core::cArray;

BEGIN_NAMESPACE( MoMa )

class cPCreature;

class MOMA_API cCreature : public IOgreListenerEvent
{
protected:
	cPCreature *mPrototype;

	Ogre::Vector3 mPosition;

	cArray<Ogre::Billboard*> mBillboards;
	cArray<Ogre::Vector2> mBillboardSizes;
	cArray<Ogre::Vector3> mBillboardPositions;
	Ogre::Billboard *mEyeBillboard;

	cMotion<Ogre::Vector3> mEyeOffset;
	cMotion<float> mEyeDistortion;
	cMotion<float> mEyeSize;
	cMotion<float> mEyeType;
	cArray< cMotion<float>* > mTransients;
	cMotion<float> mBlink;
	cMotion<Ogre::Vector2> mHeadOffset;
	cMotion<float> mHeadDistortion;

	Ogre::Vector2 mScale;

	unsigned long mLastUpdate;
	float mTime;

	void UpdateTransients( const float iEllapsed );

public:
	cCreature( cPCreature *iProto, const Ogre::Vector3 &iPosition );

	void SetEyeRotation( const float iYaw, const float iPitch );
	const Ogre::Vector3 GetEyeTarget();
	void SetEyeTarget(Ogre::Vector3 vTarget);
	void SetEyeDistortion( const float iStrength );
	void SetEyeSize( const float iSize );
	void SetEyeType( const float iType );	// [0..1]
	float GetEyeSize( );
	void Blink( const float iSpeed );
	void SetHeadDistortionPosition( const Ogre::Vector2 &iPosition );
	void SetHeadDistortionStrength( const float iStrength );
	void SetScale( const Ogre::Vector2 &iScale );
	void SetPosition( const Ogre::Vector3 &iPosition );

	void StartTransient( const float iSpeed );

	// IOgreListenerEvent methods
	virtual void OnRender( void );

	float mEyeSpeed;
	float mHeadSpeed;
	float mTransientSize;
	float mTransientStrength;
};

END_NAMESPACE()
