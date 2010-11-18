#pragma once

#include <Core/Datatype/cAutoPtr.h>

using Loom::Core::cAutoPtr;

namespace Ogre
{
	class BillboardSet;
};

BEGIN_NAMESPACE( MoMa )

class cCircle;

class MOMA_API cPCircle
{
protected:
	Ogre::BillboardSet *mBillboardSet;

public:
	cPCircle();
	~cPCircle();

	cCircle *CreateInstance( void );
	Ogre::BillboardSet *GetBillboardSet( void ) const { return mBillboardSet; }
};

END_NAMESPACE()
