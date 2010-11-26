#pragma once

#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Core/Datatype/cAutoPtr.h>

using Loom::OgreApp::IOgreListenerEvent;
using Loom::Core::cAutoPtr;
using Loom::Core::cObject;

namespace Ogre
{
	class BillboardSet;
};

BEGIN_NAMESPACE( MoMa )

class cWaveform : public IOgreListenerEvent, public cObject<cWaveform>
{
protected:
	Ogre::BillboardSet *mBillboardSet[2];

	unsigned long mLastUpdate;
	size_t mNumParts;
	size_t mFirstPart;
	cAutoPtr<float> mHeights;
	cAutoPtr<float> mTargetHeights;
	float mMockInput;

public:
	float mInertia;
	float mFrequency;
	float mSpeed;
	float mInertiaDamping;
	float mMockDamping;

public:
	cWaveform();

	void Update( void );

	// IOgreListenerEvent methods
	virtual void OnRender( void );
};

END_NAMESPACE()