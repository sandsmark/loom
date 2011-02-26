#pragma once

#include <Core/Container/cArray.h>

namespace Ogre
{
	class Billboard;
	class ColourValue;
	class Radian;
	class Vector2;
};

using Loom::Core::cArray;

BEGIN_NAMESPACE( MoMa )

class cPCircle;

class MOMA_API cCircle
{
protected:
	cPCircle *mPrototype;
	cArray<Ogre::Billboard*> mBillboards;
	Ogre::Billboard *mBillboard;

public:
	cCircle( cPCircle *iProto );

	// Colour.a controls dim/glow
	// alpha = 0   : full dimm
	// alpha = 0.5 : normal display
	// alpha = 1   : full glow
	void SetColour( const Ogre::ColourValue &iColour );
	void SetSkew( const float iSkew );
	void SetRotation( const Ogre::Radian &iRotation );
	void SetSize( const Ogre::Vector2 &iSize );
};

END_NAMESPACE()
