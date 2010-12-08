#pragma once

#include <Core/Container/cArray.h>
#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Ogre/OgreVector3.h>

using Loom::OgreApp::IOgreListenerEvent;

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
	cArray<Ogre::Billboard*> mBillboards;
	Ogre::Billboard *mEyeBillboard;

	Ogre::Vector3 mEyeOffset;
	Ogre::Vector3 mEyeTarget;
	unsigned long mLastUpdate;

	float mDistortionStrength;
	float mDistortionTarget;

public:
	cCreature( cPCreature *iProto, const Ogre::Vector3 &iPosition );

	void RotateEye( float iYaw, float iPitch );
	void SetDistortionStrength( float iStrength );

	// IOgreListenerEvent methods
	virtual void OnRender( void );

	float mEyeSpeed;
};

END_NAMESPACE()
