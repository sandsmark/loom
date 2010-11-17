#pragma once

#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Core/Datatype/cAutoPtr.h>

using Loom::OgreApp::IOgreListenerEvent;
using Loom::Core::cAutoPtr;

namespace Ogre
{
	class BillboardSet;
};

BEGIN_NAMESPACE( MoMa )

class cWaveform : public IOgreListenerEvent
{
protected:
	Ogre::BillboardSet *mBillboardSet;
	unsigned long mLastUpdate;
	size_t mNumParts;
	size_t mFirstPart;
	cAutoPtr<float> mHeights;
	cAutoPtr<float> mTargetHeights;
	float mInertia;

	float mMockInput;

public:
	cWaveform();

	void Update( void );

	// IOgreListenerEvent methods
	virtual void OnRender( void );
};

END_NAMESPACE()