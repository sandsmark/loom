#pragma once

#include <Core/Container/cArray.h>

namespace Ogre
{
	class Billboard;
};

using Loom::Core::cArray;

BEGIN_NAMESPACE( MoMa )

class cPCircle;

class MOMA_API cCircle
{
protected:
	cPCircle *mPrototype;
	cArray<Ogre::Billboard*> mBillboards;

public:
	cCircle( cPCircle *iProto );
};

END_NAMESPACE()
