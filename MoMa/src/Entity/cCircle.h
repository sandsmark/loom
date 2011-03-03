#pragma once

#include <Core/Container/cArray.h>
#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>

#include <Ogre/OgreColourValue.h>
#include <Ogre/OgreVector2.h>
#include <Ogre/OgreVector3.h>

namespace Ogre
{
	class Billboard;
};

using Loom::Core::cArray;
using Loom::OgreApp::IOgreListenerEvent;

BEGIN_NAMESPACE( MoMa )

class cPCircle;

class MOMA_API cCircle : public IOgreListenerEvent
{
protected:
	cPCircle *mPrototype;
	cArray<Ogre::Billboard*> mBillboards;
	Ogre::Billboard *mBillboard;

	Ogre::ColourValue mColour;
	Ogre::Vector2 mDimensions;
	Ogre::Vector3 mPosition;
	Ogre::Radian mRotation;
	volatile bool mDirty;

	bool mInited;
	void Init( void );

public:
	cCircle( cPCircle *iProto );

	// Colour.a controls dim/glow
	// alpha = 0   : full dimm
	// alpha = 0.5 : normal display
	// alpha = 1   : full glow
	void SetColour( const Ogre::ColourValue &iColour );
	const Ogre::ColourValue& GetColour( void );
	void SetSkew( const float iSkew );
	void SetRotation( const Ogre::Radian &iRotation );
	void SetSize( const Ogre::Vector2 &iSize );
	void SetPosition( const Ogre::Vector2 &iPosition );

	// IOgreListenerEvent methods
	virtual void OnRender( void );
};

END_NAMESPACE()
