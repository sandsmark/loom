#pragma once

#include <Core/Container/cArray.h>

namespace Ogre
{
	class Billboard;
};

using Loom::Core::cArray;

BEGIN_NAMESPACE( MoMa )

class cPCreature;

class MOMA_API cCreature
{
protected:
	cPCreature *mPrototype;
	cArray<Ogre::Billboard*> mBillboards;

public:
	cCreature( cPCreature *iProto );
};

END_NAMESPACE()
