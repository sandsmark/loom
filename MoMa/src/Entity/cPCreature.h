#pragma once

#include <Core/Datatype/cAutoPtr.h>

using Loom::Core::cAutoPtr;

namespace Ogre
{
	class BillboardSet;
	class Vector3;
};

BEGIN_NAMESPACE( MoMa )

class cCreature;

class MOMA_API cPCreature
{
public:
	enum eParts
	{
		PART_BODY = 0,
		PART_EYE,

		PART_MAX,
	};
protected:
	Ogre::BillboardSet *mBillboardSet[PART_MAX];
	size_t mNumParts;

public:
	cPCreature();
	~cPCreature();

	cCreature *CreateInstance( const Ogre::Vector3 &iPosition );
	Ogre::BillboardSet *GetBillboardSet( const eParts iPart ) const { return mBillboardSet[iPart]; }
	size_t GetNumParts( void ) const { return mNumParts; }
};

END_NAMESPACE()
