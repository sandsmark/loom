#include <MoMa/Entity/cCircle.h>
#include <MoMa/Entity/cPCircle.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>

using namespace Loom::MoMa;

/************************************************************************/
cCircle::cCircle( cPCircle *iProto )
: mPrototype( iProto )
/************************************************************************/
{
	Ogre::BillboardSet *vBillboardSet = mPrototype->GetBillboardSet();
	Ogre::Billboard *vBillboard = vBillboardSet->createBillboard( Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( 0, 1000 ) );
	float vSize = Ogre::Math::RangeRandom( 20, 70 );
	vBillboard->setDimensions( vSize, vSize );
	mBillboards.Add( vBillboard );
}
