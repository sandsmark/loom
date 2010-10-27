#include <MoMa/Entity/cCreature.h>
#include <MoMa/Entity/cPCreature.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>

using namespace Loom::MoMa;

/************************************************************************/
cCreature::cCreature( cPCreature *iProto )
: mPrototype( iProto )
/************************************************************************/
{
	int vNumParts = mPrototype->GetNumParts();

	// Body
	Ogre::BillboardSet *vBodySet = mPrototype->GetBillboardSet( cPCreature::PART_BODY );
	for ( int i=0; i<vNumParts; i++ )
	{
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( 0, -( -5 + i ), 0 );
		vBillboard->setDimensions( ( sinf( i * 0.14f ) * 5.0f + 5.0f + Ogre::Math::RangeRandom( 0.5f, 5 ) ) * 3, 1 );
		mBillboards.Add( vBillboard );
	}

	// Eye
	Ogre::BillboardSet *vEyeSet = mPrototype->GetBillboardSet( cPCreature::PART_EYE );
	{
		Ogre::Billboard *vBillboard = vEyeSet->createBillboard( 0, -( -5 + 7 ), -0.1f );
		vBillboard->setDimensions( 15, 15 );
		mBillboards.Add( vBillboard );
	}
}
